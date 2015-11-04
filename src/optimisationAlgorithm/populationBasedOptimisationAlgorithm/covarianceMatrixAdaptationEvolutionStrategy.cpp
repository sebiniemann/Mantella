#include <mantella_bits/optimisationAlgorithm/populationBasedOptimisationAlgorithm/covarianceMatrixAdaptationEvolutionStrategy.hpp>

// C++ standard library
#include <cassert>
#include <cmath>
#include <algorithm>

// Mantella
#include <mantella_bits/helper/assert.hpp>
#include <mantella_bits/helper/statistic.hpp>
#include <mantella_bits/optimisationProblem.hpp>

namespace mant {

  CovarianceMatrixAdaptationEvolutionStrategy::CovarianceMatrixAdaptationEvolutionStrategy(
      const std::shared_ptr<OptimisationProblem> optimisationProblem,
      const arma::uword populationSize)
  : PopulationBasedOptimisationAlgorithm(optimisationProblem, populationSize) {
    //HCMA settings coming from xacmes.m - 
    setStartingPoint(getRandomParameter());
    setStepSize((getUpperBounds() - getLowerBounds()) / 5.0);
    setToleranceFun(1e-12);
    setToleranceHistFun(1e-12);
    setToleranceX(2e-12);
    setToleranceUpX(arma::datum::inf);
    setIRun(0);
    activeCMA_ = 0;
    countiter_ = 0;
    stopOnStagnation_ = true;
    stopOnWarnings_ = true;
    stopOnEqualFunctionValues_ = true;

    xmean_ = startingPoint_;
    xold_ = startingPoint_;
  }

  CovarianceMatrixAdaptationEvolutionStrategy::CovarianceMatrixAdaptationEvolutionStrategy(
      const std::shared_ptr<OptimisationProblem> optimisationProblem)
  : CovarianceMatrixAdaptationEvolutionStrategy(optimisationProblem,
  4 + std::floor(3 * std::log(optimisationProblem->numberOfDimensions_))) {
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::initializeRun() {
    //TODO: opts.IncPopSize is actually (somewhat) used in HCMA when Bipop is activated. (see xacmes.m)
    //Still completely remove it?
    lambda_last_ = populationSize_;
    this->setPopulationSize(this->populationSize_);

    sigma_ = arma::max(stepSize_);
    pc_ = arma::zeros(numberOfDimensions_);
    ps_ = arma::zeros(numberOfDimensions_);

    diagD_ = stepSize_ / sigma_;
    diagC_ = arma::square(diagD_);
    B_ = arma::eye(numberOfDimensions_, numberOfDimensions_); //;B defines the coordinate system
    BD_ = arma::diagmat(diagD_); //;B*D for speed up only
    C_ = arma::diagmat(diagC_); //;covariance matrix == BD*(BD)'

    chiN_ = std::pow(numberOfDimensions_, 0.5) * (1 - 1.0 / (4 * numberOfDimensions_) + 1.0 / (21 * std::pow(numberOfDimensions_, 2)));
    //;expectation of||N(0,I)|| == norm(randn(N,1))

    //miscellaneous inits needed
    runInitialized_ = true;
    EqualFunctionValues_ = arma::zeros(10 + numberOfDimensions_);
    stopMaxIter_ = 100 + 50 * std::pow(numberOfDimensions_ + 3, 2) / std::sqrt(populationSize_);
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::optimiseImplementation() {
    //do some necessary initialization stuff
    if (!runInitialized_) {
      initializeRun();
    }

    //TODO: this stopflag is more sophisticated in the matlab code.
    bool stopFlag = false;

    while (!stopFlag && !this->singleIteration_) {
      std::cout << "while iter: " << this->countiter_ << std::endl;
      //;set internal parameters
      if (this->populationSize_ != this->lambda_last_) {
        this->setPopulationSize(this->populationSize_);
      }

      this->countiter_++;

      //;Generate and evaluate lambda offspring
      this->fitnessRaw_ = arma::ones(this->populationSize_);

      std::cout << "newgen" << std::endl;
      //;parallel evaluation - the if is omitted.
      arma::Mat<double> newGenerationRaw = arma::randn(this->numberOfDimensions_, this->populationSize_); //arz
      arma::Mat<double> newGeneration = static_cast<arma::Mat<double>>(this->sigma_ * (this->BD_ * newGenerationRaw)).each_col() + this->xmean_; //arx

      std::cout << "capping" << std::endl;
      this->newGenerationValid_ = std::get<0>(this->capToBoundary(newGeneration)); //arxvalid

      //TODO: cmaes passes the whole matrix into the fitnessfunction, not sure if this is the right way around it
      for (arma::uword i = 0; i < this->populationSize_; i++) {
        this->fitnessRaw_(i) = this->getObjectiveValue(this->newGenerationValid_.col(i));
        this->numberOfIterations_++;
      }

      std::cout << "boundaries" << std::endl;
      //;----- handle boundaries -----
      //;Assigned penalized fitness
      arma::Col<double> boundaryPenalty = arma::sum(arma::abs(newGenerationValid_ - newGeneration)).t();
      fitnessSel_ = fitnessRaw_ + boundaryPenalty;
      //;----- end handle boundaries -----

      //;Sort by fitness
      std::cout << "fitness" << std::endl;
      fitnessIdx_ = arma::sort_index(fitnessRaw_);
      fitnessRaw_ = arma::sort(fitnessRaw_);
      fitnessIdxSel_ = arma::sort_index(fitnessSel_);
      fitnessSel_ = arma::sort(fitnessSel_); //;minimization

      std::cout << "xmean" << std::endl;
      //;Calculate new xmean, this is selection and recombination 
      xold_ = xmean_; //;for speed up of Eq. (2) and (3)
      xmean_ = newGeneration.cols(fitnessIdxSel_.rows(0, mu_ - 1)) * recombinationWeights_;
      arma::Mat<double> zmean = newGenerationRaw.cols(fitnessIdxSel_.rows(0, mu_ - 1)) * recombinationWeights_; //;==D^-1*B'*(xmean-xold)/sigma

      std::cout << "ps/pc" << std::endl;
      //;Cumulation: update evolution paths
      ps_ = (1 - cs_) * ps_ + std::sqrt(cs_ * (2 - cs_) * mueff_) * (B_ * zmean); //;Eq. (4)
      double hsig = arma::norm(ps_) / std::sqrt(1 - std::pow(1 - cs_, 2 * countiter_)) / chiN_ < 1.4 + 2.0 / (numberOfDimensions_ + 1);

      pc_ = (1 - ccum_) * pc_ + hsig * (std::sqrt(ccum_ * (2 - ccum_) * mueff_) / sigma_) * (xmean_ - xold_); //;Eq. (2)

      std::cout << "c upd" << std::endl;
      //;Adapt covariance matrix
      negCcov_ = 0;
      if ((ccov1_ + ccovmu_) > 0) {
        arma::Mat<double> arpos = static_cast<arma::Mat<double>>(newGeneration.cols(fitnessIdxSel_.rows(0, mu_ - 1))).each_col() - (xold_ / sigma_);
        //;"active" CMA update: negative update, in case controlling pos. definiteness 
        if (activeCMA_ > 0) {
          //;set parameters
          double negMu = mu_;
          double negMueff = mueff_;
          if (activeCMA_ > 10) {//;flat weights with mu=lambda/2
            negMu = std::floor(populationSize_ / 2);
            negMueff = negMu;
          }
          /*;
          % ;neg.mu = ceil(min([N, lambda/4, mueff]));  neg.mueff = mu; % i.e. neg.mu <= N 
          % ;Parameter study: in 3-D lambda=50,100, 10-D lambda=200,400, 30-D lambda=1000,2000 a 
          % ;three times larger neg.ccov does not work. 
          % ;  increasing all ccov rates three times does work (probably because of the factor (1-ccovmu))
          % ;  in 30-D to looks fine
           */

          std::cout << "negccov" << std::endl;
          negCcov_ = (1 - ccovmu_) * 0.25 * negMueff / (std::pow(numberOfDimensions_ + 2, 1.5) + 2 * negMueff);
          double negMinResidualVariance = 0.66; //;keep at least 0.66 in all directions, small popsize are most critical
          double negAlphaOld = 0.5; //;where to make up for the variance loss, 0.5 means no idea what to do
          //;1 is slightly more robust and gives a better "guaranty" for pos. def., 
          //;but does it make sense from the learning perspective for large ccovmu?

          double negCcovFinal = negCcov_;

          //;prepare vectors, compute negative updating matrix Cneg and checking matrix Ccheck
          //have to do some workaround since you cannot create negative descending sequences with arma
          std::cout << "neghelper" << std::endl;
          arma::Col<arma::uword> negHelper = arma::conv_to<arma::uvec>::from(
                  -arma::linspace(-(populationSize_*1.0 - 1), -(populationSize_*1.0 - negMu), std::abs(negMu)));
          arma::Col<arma::uword> temp = fitnessIdxSel_.rows(negHelper);
          arma::Mat<double> newGenerationRawNeg = newGenerationRaw.cols(fitnessIdxSel_.rows(negHelper)); //arzneg
          //;i-th longest becomes i-th shortest
          
          arma::Col<arma::uword> ngRawNegNormIndex = arma::sort_index(arma::sqrt(arma::sum(arma::square(newGenerationRawNeg), 0))); //idxnorms
          arma::Col<double> ngRawNegNorm = arma::sort(arma::sqrt(arma::sum(arma::square(newGenerationRawNeg), 0))).t(); //arnorms
          ngRawNegNormIndex = arma::sort_index(ngRawNegNormIndex); //;inverse index
          //another workaround for negative ordering
          std::cout << "neghelper2" << std::endl;
          negHelper = arma::conv_to<arma::uvec>::from(
                  -arma::linspace(-(ngRawNegNorm.n_elem*1.0 - 1),0,ngRawNegNorm.n_elem));
          arma::Col<double> ngRawNegNormFacs = ngRawNegNorm(negHelper) / ngRawNegNorm; //arnormfacs
          ngRawNegNorm = ngRawNegNormFacs; //;for the record
          if (activeCMA_ < 20) {
            newGenerationRawNeg = newGenerationRawNeg.each_row() % ngRawNegNormFacs.t(); //;E x*x' is N
          }
          std::cout << "ccheck" << std::endl;
          arma::Mat<double> Ccheck; //intentional spelling
          arma::Mat<double> Cneg;
          if (activeCMA_ < 10 && negMu == mu_) { //;weighted sum
            if (activeCMA_ % 10 == 1) {
              Ccheck = newGenerationRawNeg * arma::diagmat(recombinationWeights_) * newGenerationRawNeg.t(); //;in order to check the largest EV
            }
            Cneg = BD_ * newGenerationRawNeg * arma::diagmat(recombinationWeights_) * (BD_ * newGenerationRawNeg).t();
          } else { //; simple sum
            if (activeCMA_ % 10 == 1) {
              Ccheck = (1.0 / negMu) * newGenerationRawNeg * newGenerationRawNeg.t(); //;in order to check largest EV
            }
            Cneg = (1.0 / negMu) * BD_ * newGenerationRawNeg * (BD_ * newGenerationRawNeg).t();
          }

          std::cout << "activecma" << std::endl;
          //;check pos.def. and set learning rate neg.ccov accordingly, 
          //;this check makes the original choice of neg.ccov extremly failsafe 
          //;still assuming C == BD*BD', which is only approxim. correct 
          if (activeCMA_ % 10 == 1 &&
              arma::all(1 - negCcov_ * arma::square(ngRawNegNorm(ngRawNegNormIndex)).t() * recombinationWeights_
              < negMinResidualVariance)) {
            //In matlab there is a long block of comments here about how to calculate these two values
            //TODO: matlab apparently doesn't care that the eigenvalue could be complex.
            //Not sure if simply grabbing the real part here is correct.
            double maxeigenval = arma::max(arma::eig_gen(Ccheck)).real();
            negCcovFinal = std::min(negCcov_, (1 - ccovmu_)*(1 - negMinResidualVariance) / maxeigenval);
          }
          std::cout << "update c" << std::endl;
          //;update C
          C_ = (1 - ccov1_ - ccovmu_ + negAlphaOld * negCcovFinal + (1 - hsig) * ccov1_ * ccum_ * (2 - ccum_)) * C_ //;regard old matrix
              + ccov1_ * pc_ * pc_.t() //;plus rank one update
              + (ccovmu_ + (1 - negAlphaOld) * negCcovFinal) //;plus rank mu update
              * arpos * (static_cast<arma::Mat<double>>(arpos.t()).each_col() % recombinationWeights_)
              - negCcovFinal * Cneg; //;minus rank mu update
        } else { //; no active (negative) update
          std::cout << "no active c" << std::endl;
          C_ = (1 - ccov1_ - ccovmu_ + (1 - hsig) * ccov1_ * ccum_ * (2 - ccum_)) * C_ //;regard old matrix
              + ccov1_ * pc_ * pc_.t() //;plus rank one update
              + ccovmu_ * arpos * (static_cast<arma::Mat<double>>(arpos.t()).each_col() % recombinationWeights_); //;plus rank mu update
        }
        std::cout << "diagc" << std::endl;
        diagC_ = arma::diagvec(C_);
      }

      std::cout << "adapt sigma" << std::endl;
      //;Adapt sigma
      sigma_ = sigma_ * std::exp(
          std::min(1.0,
          (std::sqrt(arma::accu(arma::square(ps_))) / chiN_ - 1) * cs_ / damping_)); //; Eq. (5)

      std::cout << "b/d/c" << std::endl;
      //;Update B and D from C
      if ((ccov1_ + ccovmu_ + negCcov_) > 0 && countiter_ % 1 / ((ccov1_ + ccovmu_ + negCcov_) * numberOfDimensions_ * 10) < 1) {
        std::cout << "symm c" << std::endl;
        C_ = arma::symmatu(C_); //;enforce symmetry to prevent complex numbers
        arma::Col<double> tmp;
        arma::eig_sym(tmp, B_, C_); //;eigen decomposition, B==normalized eigenvectors
        //;effort: approx. 15*N matrix-vector multiplications
        std::cout << "diagD symm:" << diagD_ << std::endl;
        std::cout << "tmp" << tmp << std::endl;
        std::cout << "diagvec(tmp)" << arma::diagvec(tmp) << std::endl;
        //Matlab returns eigenvalues as a diagonal matrix, so they do diagD = diag(tmp) here
        diagD_ = tmp;

        std::cout << "assert" << std::endl;
        assert(arma::is_finite(diagD_));
        assert(arma::is_finite(B_));

        std::cout << "condition of c" << std::endl;
        //;limit condition of C to 1e14 + 1
        //TODO: this can actually happen when an eigenvalue gets negative (which afaik can only happen if activeCMA is turned on)
        if (arma::min(diagD_) <= 0) {
          if (stopOnWarnings_) {
            stopFlag = true;
          } else {
            std::cout << "warning block" << std::endl;
            //TODO: warning gets thrown here
            //another workaround
            diagD_(arma::find(diagD_ < 0)) = arma::zeros(((arma::uvec)(arma::find(diagD_ < 0))).n_elem);
            tmp = arma::max(diagD_) / 1e14;
            C_ = C_ + tmp * arma::eye(numberOfDimensions_, numberOfDimensions_);
            diagD_ = diagD_ + tmp * arma::ones(numberOfDimensions_, 1);
          }
        }
        if (arma::max(diagD_) > 1e14 * arma::min(diagD_)) {
          if (stopOnWarnings_) {
            stopFlag = true;
          } else {
            std::cout << "other warning block" << std::endl;
            //TODO: warning gets thrown here
            tmp = arma::max(diagD_) / 1e14 - arma::min(diagD_);
            C_ = C_ + tmp * arma::eye(numberOfDimensions_, numberOfDimensions_);
            diagD_ = diagD_ + tmp * arma::ones(numberOfDimensions_, 1);
          }
        }

        std::cout << "diagc/d/bd" << std::endl;
        diagC_ = arma::diagvec(C_);
        std::cout << "diagD:" << diagD_ << std::endl;
        diagD_ = arma::sqrt(diagD_); //;D contains standard deviations now
        std::cout << "C:" << C_ << std::endl;
        std::cout << "diagC:" << diagC_ << std::endl;
        std::cout << "diagD:" << diagD_ << std::endl;
        std::cout << "B:" << B_ << std::endl;
        BD_ = B_.each_col() % diagD_;
      }

      std::cout << "numeric error" << std::endl;
      //;----- numerical error management -----
      //TODO: control these skips
      //;Adjust maximal coordinate axis deviations
      //skipped since the threshold is infinity
      //; Adjust minimal coordinate axis deviations
      //this also should never happen because of positive definite
      //the threshold here is 0
      //;Adjust too low coordinate axis deviations
      if (arma::any(xmean_ == xmean_ + 0.2 * sigma_ * arma::sqrt(diagC_))) {
        if (stopOnWarnings_) {
          stopFlag = true;
        } else {
          //TODO: warning gets thrown here
          C_ = C_ + (ccov1_ + ccovmu_) * arma::diagmat(diagC_ % (xmean_ == xmean_ + 0.2 * sigma_ * arma::sqrt(diagC_)));
          sigma_ = sigma_ * std::exp(0.05 + cs_ / damping_);
        }
      }
      std::cout << "stepsize" << std::endl;
      //;Adjust step size in case of (numerical) precision problem 
      arma::Mat<double> tmp = 0.1 * sigma_ * BD_.col(std::floor(countiter_ % numberOfDimensions_));
      if (arma::all(xmean_ == xmean_ + tmp)) {
        if (stopOnWarnings_) {
          stopFlag = true;
        } else {
          sigma_ = sigma_ * std::exp(0.2 + cs_ / damping_);
        }
      }
      std::cout << "equal func" << std::endl;
      //;Adjust step size in case of equal function values (flat fitness)
      if (fitnessSel_(0) == fitnessSel_(std::ceil(0.1 + populationSize_ / 4))) {
        if (stopOnEqualFunctionValues_) {
          EqualFunctionValues_.shed_row(EqualFunctionValues_.n_elem - 1);
          EqualFunctionValues_.insert_rows(0, countiter_);
          if (EqualFunctionValues_(EqualFunctionValues_.n_elem - 1) > countiter_ - 3 * EqualFunctionValues_.n_elem) {
            stopFlag = true;
          }
        } else {
          sigma_ = sigma_ * std::exp(0.2 + cs_ / damping_);
        }
      }
      std::cout << "stepsize2" << std::endl;
      //;Adjust step size in case of equal function values
      //replacement of fitnesshist
      if (countiter_ > 2 && arma::all(fitnessRaw_ - fitnessRawPreviousIteration_ == 0)) {
        if (stopOnWarnings_) {
          stopFlag = true;
        } else {
          sigma_ = sigma_ * std::exp(0.2 + cs_ / damping_);
        }
      }

      //;----- end numerical error management -----
      std::cout << "stopflags" << std::endl;
      //;Set stop flag
      if (fitnessRaw_(0) <= getAcceptableObjectiveValue()) {
        stopFlag = true;
      }
      if (getNumberOfIterations() >= getMaximalNumberOfIterations()) {
        stopFlag = true;
      }
      if (countiter_ >= stopMaxIter_) {
        stopFlag = true;
      }
      if (arma::all(sigma_ * (arma::abs(pc_), arma::sqrt(diagC_)) < toleranceX_)) {
        stopFlag = true;
      }
      if (arma::any(sigma_ * arma::sqrt(diagC_) > toleranceUpX_)) {
        stopFlag = true;
      }
      if (sigma_ * arma::max(diagD_) == 0) {//;should never happen
        stopFlag = true;
      }

      //TODO: in matlab iterations and funcevals are separated and have an individual limit.
      //there is a separate check here if the number of function evals is higher than the limit

      //matlab does some file writing here
      //also does some output generation and formatting
      //both of these are omitted in the modified version for HCMA and should
      //likewise not interest us

      //TODO: replacement of fitnesshist for plateau detection, so we can do the same checks as cmaes does
      //but in a less refined and data heavy way
      fitnessRawPreviousIteration_ = fitnessSel_;
      
      if(updateBestParameter(newGenerationValid_.col(fitnessIdx_(0)), fitnessRaw_(0))) {
      std::cout << "BEST OBJ:" << getBestObjectiveValue() << std::endl;
    }
    }

    //This is not done exactly as in matlab since
    if(updateBestParameter(newGenerationValid_.col(fitnessIdx_(0)), fitnessRaw_(0))) {
      std::cout << "BEST OBJ:" << getBestObjectiveValue() << std::endl;
    }
    double xmeanObjValue = getObjectiveValue(std::get<0>(capToBoundary(xmean_)));
    numberOfIterations_++;
    if(updateBestParameter(xmean_, xmeanObjValue)) {
      std::cout << "BEST OBJ:" << getBestObjectiveValue() << std::endl;
    }
  }

  arma::uword CovarianceMatrixAdaptationEvolutionStrategy::getIRun() {
    return irun_;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setIRun(const arma::uword irun) {
    this->irun_ = irun;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setStepSize(const arma::Col<double> stepSize) {
    verify(stepSize.n_elem == numberOfDimensions_, "");
    this->stepSize_ = stepSize;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setStartingPoint(const arma::Col<double> xStart) {
    verify(xStart.n_elem == numberOfDimensions_, "");
    this->startingPoint_ = xStart;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setPopulationSize(const arma::uword popSize) {
    this->populationSize_ = popSize;
    lambda_last_ = populationSize_;
    mu_ = std::floor(populationSize_ / 2.0);
    recombinationWeights_ = std::log(mu_ + 0.5) - arma::log(arma::linspace(1, mu_, mu_));
    
    mueff_ = std::pow(arma::accu(recombinationWeights_), 2) / arma::accu(arma::square(recombinationWeights_)); //;variance-effective size of mu
    recombinationWeights_ = arma::normalise(recombinationWeights_, 1); //;normalize recombination weights array
    //error check omitted, shouldn't happen

    ccum_ = std::pow((numberOfDimensions_ + 2 * mueff_ / numberOfDimensions_) / (4 + mueff_ / numberOfDimensions_), -1); //;time constant for cumulation for covariance matrix
    cs_ = (mueff_ + 2) / (numberOfDimensions_ + mueff_ + 3);

    ccov1_ = std::min(2.0, populationSize_ / 3.0) / (std::pow(numberOfDimensions_ + 1.3, 2) + mueff_);
    ccovmu_ = std::min(2.0, populationSize_ / 3.0) / (mueff_ - 2 + 1.0 / mueff_) / (std::pow(numberOfDimensions_ + 2, 2) + mueff_);

    damping_ = 1 + 2 * std::max(0.0, std::sqrt((mueff_ - 1) / (numberOfDimensions_ + 1)) - 1) + cs_;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setSingleIteration(const bool DoSingleIteration) {
    this->singleIteration_ = DoSingleIteration;
  }

  std::string CovarianceMatrixAdaptationEvolutionStrategy::toString() const noexcept {
    return "CovarianceMatrixAdaptationEvolutionStrategy";
  }

  double CovarianceMatrixAdaptationEvolutionStrategy::getCcov1() const {
    return ccov1_;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setCcov1(double ccov1) {
    this->ccov1_ = ccov1;
  }

  double CovarianceMatrixAdaptationEvolutionStrategy::getCcovmu() const {
    return ccovmu_;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setCcovmu(double ccovmu) {
    this->ccovmu_ = ccovmu;
  }

  double CovarianceMatrixAdaptationEvolutionStrategy::getCcum() const {
    return ccum_;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setCcum(double ccum) {
    this->ccum_ = ccum;
  }

  double CovarianceMatrixAdaptationEvolutionStrategy::getCs() const {
    return cs_;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setCs(double cs) {
    this->cs_ = cs;
  }

  arma::uword CovarianceMatrixAdaptationEvolutionStrategy::getMu() const {
    return mu_;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setMu(arma::uword numberOfParents) {
    this->mu_ = numberOfParents;
  }

  arma::Col<double> CovarianceMatrixAdaptationEvolutionStrategy::getRecombinationWeights() {
    return recombinationWeights_;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setRecombinationWeights(arma::Col<double> weights) {
    this->recombinationWeights_ = weights;
  }

  double CovarianceMatrixAdaptationEvolutionStrategy::getToleranceFun() const {
    return toleranceFun_;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setToleranceFun(double toleranceFun) {
    this->toleranceFun_ = toleranceFun;
  }

  double CovarianceMatrixAdaptationEvolutionStrategy::getToleranceHistFun() const {
    return toleranceHistFun_;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setToleranceHistFun(double toleranceHistFun) {
    this->toleranceHistFun_ = toleranceHistFun;
  }

  double CovarianceMatrixAdaptationEvolutionStrategy::getToleranceUpX() const {
    return toleranceUpX_;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setToleranceUpX(double toleranceUpX) {
    this->toleranceUpX_ = toleranceUpX;
  }

  double CovarianceMatrixAdaptationEvolutionStrategy::getToleranceX() const {
    return toleranceX_;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setToleranceX(double toleranceX) {
    this->toleranceX_ = toleranceX;
  }

  arma::Col<double> CovarianceMatrixAdaptationEvolutionStrategy::getXmean() const {
    return xmean_;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setXmean(arma::Col<double> xmean) {
    this->xmean_ = xmean;
  }

  //returns capped matrix/vector first, indexes of capped values second
  std::tuple<arma::Mat<double>, arma::Mat<double>> CovarianceMatrixAdaptationEvolutionStrategy::capToBoundary(arma::Mat<double> x) {
    arma::Mat<double> indexes = arma::zeros(x.n_rows, x.n_cols);
    for (arma::uword n = 0; n < x.n_cols; ++n) {
      const arma::Col<arma::uword>& lowerIndex = arma::find(x.col(n) < getLowerBounds());
      static_cast<arma::Col<double>>(x.col(n)).elem(lowerIndex) = getLowerBounds().elem(lowerIndex);
      indexes(lowerIndex) += -1;
      
      const arma::Col<arma::uword>& upperIndex = arma::find(x.col(n) > getUpperBounds());
      static_cast<arma::Col<double>>(x.col(n)).elem(upperIndex) = getUpperBounds().elem(upperIndex);
      indexes(upperIndex) += 1;
    }

    return std::make_tuple(x, indexes);
  }
}
