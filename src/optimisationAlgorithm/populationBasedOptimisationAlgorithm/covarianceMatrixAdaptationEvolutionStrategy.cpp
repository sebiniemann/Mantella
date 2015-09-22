#include <mantella_bits/optimisationAlgorithm/populationBasedOptimisationAlgorithm/covarianceMatrixAdaptationEvolutionStrategy.hpp>

namespace mant {

  CovarianceMatrixAdaptationEvolutionStrategy::CovarianceMatrixAdaptationEvolutionStrategy(
      const std::shared_ptr<OptimisationProblem> optimisationProblem,
      const arma::uword populationSize) noexcept
  : PopulationBasedOptimisationAlgorithm(optimisationProblem, populationSize) {
    //HCMA settings coming from xacmes.m - 
    setStartingPoint(-4 + 8 * arma::randu(this->numberOfDimensions_));
    setStepSize(arma::Col<double>{2.0});

    xmean = startingPoint;
  }

  CovarianceMatrixAdaptationEvolutionStrategy::CovarianceMatrixAdaptationEvolutionStrategy(
      const std::shared_ptr<OptimisationProblem> optimisationProblem) noexcept
  : PopulationBasedOptimisationAlgorithm(optimisationProblem,
      4 + std::floor(3 * log(this->numberOfDimensions_))) {

    //TODO: is the variable passed to parentclass above already initialized at that point?

    //HCMA settings coming from xacmes.m - 
    setStartingPoint(-4 + 8 * arma::randu(this->numberOfDimensions_));
    setStepSize(arma::Col<double>{2.0});

    xmean = startingPoint;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::initializeRun() {
    //TODO: figure out what to do with lambda0 and popsize settings
    //this overrides constructor settings on restart runs
    //it gets split into lambda and popsize, but neither variable is written to ever again in CMAES
    //xacmes also writes these variables again...
    lambda0 = std::floor(this->populationSize_ * std::pow(incPopSize, irun - 1));
    this->setPopulationSize(lambda0);

    sigma = arma::max(stepSize);
    pc = arma::zeros(this->numberOfDimensions_);
    ps = arma::zeros(this->numberOfDimensions_);

    if (stepSize.n_elem == 1) {
      stepSize = arma::ones(this->numberOfDimensions_) * stepSize(0);
    }
    diagD = stepSize / arma::max(stepSize);
    diagC = arma::pow(diagD, 2);
    B = arma::eye(this->numberOfDimensions_, this->numberOfDimensions_); //;B defines the coordinate system
    BD = B % arma::repmat(diagD.t(), this->numberOfDimensions_, 1); //;B*D for speed up only
    C = arma::diagmat(diagC); //;covariance matrix == BD*(BD)'

    //TODO: cmaes_initializeRun initialized fitness history here, but we have that already in mantella

    /////BOUNDARY
    boundaryActive = arma::any(this->getLowerBounds() > -arma::datum::inf) || arma::any(this->getUpperBounds() > -arma::datum::inf);
    if (boundaryActive) {
      //sanity check omitted, is done in mantella else where (?)
      //TODO: sanity check for initial point?
      boundaryWeights = arma::zeros(this->numberOfDimensions_);

      boundaryScale = arma::ones(this->numberOfDimensions_);

      //mark all dimensions which have a boundary
      boundaryExists = arma::Col<bool>(this->numberOfDimensions_);
      for (int i = 0; i < this->getLowerBounds().n_elem; i++) {
        if (this->getLowerBounds()(i) != arma::datum::inf || this->getUpperBounds()(i) != arma::datum::inf) {
          boundaryExists(i) = true;
        } else {
          boundaryExists(i) = false;
        }
      }
      //TODO: cmaes modifies sigma(stepsize) here if it is too small or too large, do we want that?
      boundaryDeltaFitHistory = arma::ones(1); //gets elongated later
      boundaryInitialPhase = true;
    }

    //TODO: cmaes evaluates starting point once here for output and caches that in evaluation history,
    //seems unnecessary (?)

    chiN = std::pow(this->numberOfDimensions_, 0.5) * (1 - 1.0 / (4 * this->numberOfDimensions_) + 1.0 / (21 * std::pow(this->numberOfDimensions_, 2)));
    //;expectation of||N(0,I)|| == norm(randn(N,1))

    runInitialized = true;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::optimiseImplementation() {
    //do some necessary initialization stuff
    if (!runInitialized) {
      initializeRun();
    }

    bool stopFlag = false;
    arma::uword lambda_last = this->populationSize_;
    while (!stopFlag) {
      //TODO: lambda_last??? see studip
      //;set internal parameters
      if (countiter == 0 || this->populationSize_ != lambda_last) {
        lambda_last = this->populationSize_;
        mu = std::floor(this->populationSize_ / 2.0);
        recombinationWeights = arma::zeros(mu);
        if (recombinationWeightsType == 0) {//equal
          recombinationWeights = arma::ones(mu);
        } else if (recombinationWeightsType == 1) {//linear
          recombinationWeights = mu + 0.5 - arma::linspace(1, mu, 1).t();
        } else if (recombinationWeightsType == 2) {//superlinear
          recombinationWeights = std::log(mu + 0.5) - arma::log(arma::linspace(1, mu, 1)).t();
          //;muXone array for weighted recombination
          //;qqq mu can be non-integer and
          //;should become ceil(mu-0.5) (minor correction)
        } else {
          //TODO: CMAES throws error here cause type not implemented
        }
        mueff = std::pow(arma::sum(recombinationWeights), 2) / arma::sum(arma::pow(recombinationWeights, 2)); //;variance-effective size of mu
        recombinationWeights = recombinationWeights / arma::sum(recombinationWeights); //;normalize recombination weights array
        //error check omitted, shouldn't happen

        //TODO: these values are from HCMA, standard CMAES are different. not sure how to impl
        ccum = std::pow((this->numberOfDimensions_ + 2 * mueff / this->numberOfDimensions_) / (4 + mueff / this->numberOfDimensions_), -1); //;time constant for cumulation for covariance matrix
        cs = (mueff + 2) / (this->numberOfDimensions_ + mueff + 3);

        ccov1 = std::min(2, this->populationSize_ / 3.0) / (std::pow(this->numberOfDimensions_ + 1.3, 2) + mueff);
        ccovmu = std::min(2, this->populationSize_ / 3.0) / (mueff - 2 + 1.0 / mueff) / (std::pow(this->numberOfDimensions_ + 2, 2) + mueff);

        damping = 1 + 2 * std::max(0, std::sqrt((mueff - 1) / (this->numberOfDimensions_ + 1)) - 1) + cs;
      }

      countiter++;

      //;Generate and evaluate lambda offspring
      fitnessRaw = arma::repmat(arma::datum::nan, 1, this->populationSize_);

      arma::Mat<double> newGenerationRaw = getRandomPopulation(); //arz
      arma::Mat<double> newGeneration = arma::repmat(xmean, 1, this->populationSize_) + sigma * (BD * newGenerationRaw); //arx

      arma::Mat<double> newGenerationValid;
      if (!boundaryActive) {
        newGenerationValid = newGeneration;
      } else {
        newGenerationValid = capToBoundary(newGeneration);
      }

    }
  }

  arma::uword CovarianceMatrixAdaptationEvolutionStrategy::getIRun() {
    return this->irun;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setIRun(const arma::uword irun) {
    this->irun = irun;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setLambda0(const double lambda0) {
    this->lambda0 = lambda0;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setStepSize(const arma::Col<double> sigma) {
    //TODO: length check
    this->stepSize = sigma;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setStartingPoint(const arma::Col<double> xStart) {
    //TODO: length check
    this->startingPoint = xStart;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setPopulationSize(const arma::uword popSize) {
    this->populationSize_ = popSize;
  }

  double CovarianceMatrixAdaptationEvolutionStrategy::getIncPopSize() const {
    return incPopSize;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setSingleIteration(const bool DoSingleIteration) {
    this->singleIteration = DoSingleIteration;
  }

  std::string CovarianceMatrixAdaptationEvolutionStrategy::toString() const noexcept {
    return "CovarianceMatrixAdaptationEvolutionStrategy";
  }

  double CovarianceMatrixAdaptationEvolutionStrategy::getCcov1() const {
    return ccov1;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setCcov1(double ccov1) {
    this->ccov1 = ccov1;
  }

  double CovarianceMatrixAdaptationEvolutionStrategy::getCcovmu() const {
    return ccovmu;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setCcovmu(double ccovmu) {
    this->ccovmu = ccovmu;
  }

  double CovarianceMatrixAdaptationEvolutionStrategy::getCcum() const {
    return ccum;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setCcum(double ccum) {
    this->ccum = ccum;
  }

  double CovarianceMatrixAdaptationEvolutionStrategy::getCs() const {
    return cs;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setCs(double cs) {
    this->cs = cs;
  }

  arma::uword CovarianceMatrixAdaptationEvolutionStrategy::getMu() const {
    return mu;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setMu(arma::uword numberOfParents) {
    this->mu = numberOfParents;
  }

  double CovarianceMatrixAdaptationEvolutionStrategy::getToleranceFun() const {
    return toleranceFun;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setToleranceFun(double toleranceFun) {
    this->toleranceFun = toleranceFun;
  }

  double CovarianceMatrixAdaptationEvolutionStrategy::getToleranceHistFun() const {
    return toleranceHistFun;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setToleranceHistFun(double toleranceHistFun) {
    this->toleranceHistFun = toleranceHistFun;
  }

  double CovarianceMatrixAdaptationEvolutionStrategy::getToleranceUpX() const {
    return toleranceUpX;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setToleranceUpX(double toleranceUpX) {
    this->toleranceUpX = toleranceUpX;
  }

  double CovarianceMatrixAdaptationEvolutionStrategy::getToleranceX() const {
    return toleranceX;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setToleranceX(double toleranceX) {
    this->toleranceX = toleranceX;
  }

  arma::Col<double> CovarianceMatrixAdaptationEvolutionStrategy::getXmean() const {
    return xmean;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setXmean(arma::Col<double> xmean) {
    this->xmean = xmean;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::capToBoundary(arma::Mat<double> x) {
    for (int i = 0; i < x.n_cols; i++) {
      //another workaround. subview col doesn't have eleme method...
      arma::Col<double> curCol = x.col(i);
      arma::Col<uint> violatingIndexes = arma::find(curCol < getLowerBounds()(i));
      //workaround since arma doesnt allow single value assign... but single value everything else...
      arma::Col<double> workaroundCol = arma::ones(violatingIndexes.n_elem);
      workaroundCol *= getLowerBounds()(i);
      if (violatingIndexes.n_elem > 0) {
        curCol.elem(violatingIndexes) = workaroundCol;
      }
      x.col(i) = curCol;
    }
    for (int i = 0; i < x.n_cols; i++) {
      //another workaround. subview col doesn't have eleme method...
      arma::Col<double> curCol = x.col(i);
      arma::Col<uint> violatingIndexes = arma::find(curCol > getUpperBounds()(i));
      //workaround since arma doesnt allow single value assign... but single value everything else...
      arma::Col<double> workaroundCol = arma::ones(violatingIndexes.n_elem);
      workaroundCol *= getUpperBounds()(i);
      if (violatingIndexes.n_elem > 0) {
        curCol.elem(violatingIndexes) = workaroundCol;
      }
      x.col(i) = curCol;
    }
    return x;
  }
}