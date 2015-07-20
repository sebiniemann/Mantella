namespace mant {

  template <typename T>
  class CovarianceMatrixAdaptationEvolutionStrategy : public PopulationBasedOptimisationAlgorithm<T> {
  public:
    explicit CovarianceMatrixAdaptationEvolutionStrategy(
        const std::shared_ptr<OptimisationProblem<T>> optimisationProblem,
        const unsigned int populationSize) noexcept;
    
    //used (i.e. by HCMA) to compute certain values after changing them after instantiation
    void initializeRun();
    
    unsigned int getIRun();
    void setIRun(const unsigned int irun); //irun
    void setLambda0(const double lambda0); //lambda0
    void setStepSize(const arma::Col<double> sigma); //insigma
    void setStartingPoint(const arma::Col<double> xStart); //xstart
    void setPopulationSize(const unsigned int popSize);
    
    double getCcov1() const;
    void setCcov1(double ccov1);
    double getCcovmu() const;
    void setCcovmu(double ccovmu);
    double getCcum() const;
    void setCcum(double ccum);
    double getCs() const;
    void setCs(double cs);
    unsigned int getNumberOfParents() const;
    void setNumberOfParents(unsigned int numberOfParents);
    double getToleranceFun() const;
    void setToleranceFun(double toleranceFun);
    double getToleranceHistFun() const;
    void setToleranceHistFun(double toleranceHistFun);
    double getToleranceUpX() const;
    void setToleranceUpX(double toleranceUpX);
    double getToleranceX() const;
    void setToleranceX(double toleranceX);
    arma::Col<double> getXmean() const;
    void setXmean(arma::Col<double> xmean);

    void setSingleIteration(const bool DoSingleIteration);

    std::string toString() const noexcept override;
    

  protected:
    //Notation: comments always start with matlab name of variable
    //          comments after ';' are from matlab code
    //          comments after '-' are from mantella

    arma::Col<double> startingPoint; //definput.xstart
    arma::Col<double> stepSize; //definput.insigma

    double toleranceX; //defopts.TolX; stop if x-change smaller TolX
    double toleranceUpX; //defopts.TolUpX; stop if x-changes larger TolUpX
    double toleranceFun; //defopts.TolFun; stop if fun-changes smaller TolFun
    double toleranceHistFun; //defopts.TolHistFun; stop if back fun-changes smaller TolHistFun
    bool stopOnStagnation = true; //defopts.StopOnStagnation; stop when fitness stagnates for a long time
    bool stopOnWarnings = true; //defopts.StopOnWarnings
    bool stopOnEqualFunctionValues = true; //defops.stopOnEqualFunctionValues - originally 2 + N/3  % number of iterations
    //seems strange since it's never used in an integer check, just a bool check
    
    bool evalParallel = true; //defopts.EvalParallel; objective function FUN accepts NxM matrix, with M>1?
    unsigned int restarts = 9; //defopts.Restarts - HCMA default is 9
    double incPopSize; //defopts.IncPopSize; multiplier for population size before each restart
    unsigned int numberOfParents; //defopts.ParentNumber
    unsigned int recombinationWeightsType = 0; //defotps.RecombinationWeights - 0 = superlinear decrease, 1 = linear, 2 = equal
    double cs; //defopts.CMA.cs; cumulation constant for step-size
    double damping; //defopts.CMA.damps; damping for step-size
    double ccum; //defopts.CMA.ccum; 
    double ccov1; //defopts.CMA.ccov1;
    double ccovmu; //defopts.CMA.ccovmu;
    unsigned int activeCMA; ////defopts.CMA.active; active CMA 1: neg. updates with pos. def. check, 2: neg. updates
    unsigned int irun = 0; //irun
    //opts.EvalParallel - was left out since we always want to do that and there is afaik no option to even not do that in arma

    arma::Col<double> xmean; //xmean
    double lambda0; //lambda0
    double sigma; //sigma
    arma::Col<double> pc; //pc; evolution path for C
    arma::Col<double> ps; //ps; evolution path for sigma
    arma::Mat<double> B; //B
    arma::Mat<double> C; //C
    arma::Mat<double> D; //D
    arma::Mat<double> BD; //BD
    arma::Col<double> diagC; //diagC
    arma::Col<double> diagD; //diagD; diagonal matrix D defines the scaling
    double chiN; //chiN
    
    bool boundaryActive = false; //bnd.isactive
    arma::Col<double> boundaryWeights; //bnd.weights
    arma::Col<double> boundaryScale; //bnd.scale
    arma::Col<bool> boundaryExists; //bnd.isbounded
    arma::Col<double> boundaryDeltaFitHistory; //bnd.dfithist; delta fit for setting weights
    bool boundaryInitialPhase; //bnd.iniphase

    //HCMA needs to be able to do single iterations of CMAES, for that this bool can be set.
    bool singleIteration = false;
    //helper variable to check if run was initialized
    bool runInitialized = false;

    void optimiseImplementation() override;
  };

  template <typename T>
  CovarianceMatrixAdaptationEvolutionStrategy<T>::CovarianceMatrixAdaptationEvolutionStrategy(
      const std::shared_ptr<OptimisationProblem<T>> optimisationProblem,
      const unsigned int populationSize = 0) noexcept
  : PopulationBasedOptimisationAlgorithm<T>(optimisationProblem, populationSize) {
    //TODO: is defaulting popSize to 0 to use CMAES-default okay?
    if (populationSize == 0) {
      this->populationSize_ = 4 + std::floor(3 * log(this->numberOfDimensions_));
    }   

    //HCMA settings coming from xacmes.m - 
    setStartingPoint(-4 + 8 * arma::randu(this->numberOfDimensions_));
    setStepSize(arma::Col<double>{2.0});

    xmean = arma::Col<double>(this->numberOfDimensions_);
  }
  
  template <typename T>
  void CovarianceMatrixAdaptationEvolutionStrategy<T>::initializeRun() {
    numberOfParents = this->populationSize_ / 2;
    
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
    if(boundaryActive) {
      //sanity check omitted, is done in mantella else where (?)
      //TODO: sanity check for initial point?
      boundaryWeights = arma::zeros(this->numberOfDimensions_);
      
      boundaryScale = arma::ones(this->numberOfDimensions_);
      
      //mark all dimensions which have a boundary
      boundaryExists = arma::Col<bool>(this->numberOfDimensions_);
      for(int i = 0; i < this->getLowerBounds().n_elem; i++) {
        if(this->getLowerBounds()(i) != arma::datum::inf || this->getUpperBounds()(i) != arma::datum::inf) {
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
    
    chiN = std::pow(this->numberOfDimensions_,0.5) * (1-1.0/(4*this->numberOfDimensions_)+1.0/(21*std::pow(this->numberOfDimensions_,2)));
    //;expectation of||N(0,I)|| == norm(randn(N,1))
    
    runInitialized = true;
  }

  template <typename T>
  void CovarianceMatrixAdaptationEvolutionStrategy<T>::optimiseImplementation() {
    //do some necessary initialization stuff
    if(!runInitialized) {
      initializeRun();
    }
    //TODO: figure out what to do with lambda0 and popsize settings
    //-this overrides the constructor value,also it gets split into lambda and popsize,
    //but neither variable is written to ever again
    //xacmes also writes these variables again...
    lambda0 = std::floor(this->populationSize_ * std::pow(incPopSize, irun - 1));
    this->populationSize_ = lambda0;

    bool stopFlag = false;
    while (!stopFlag) {

    }
  }

  template <typename T>
  unsigned int CovarianceMatrixAdaptationEvolutionStrategy<T>::getIRun() {
    return this->irun;
  }

  template <typename T>
  void CovarianceMatrixAdaptationEvolutionStrategy<T>::setIRun(const unsigned int irun) {
    this->irun = irun;
  }

  template <typename T>
  void CovarianceMatrixAdaptationEvolutionStrategy<T>::setLambda0(const double lambda0) {
    this->lambda0 = lambda0;
  }

  template <typename T>
  void CovarianceMatrixAdaptationEvolutionStrategy<T>::setStepSize(const arma::Col<double> sigma) {
    //TODO: length check
    this->stepSize = sigma;
  }

  template <typename T>
  void CovarianceMatrixAdaptationEvolutionStrategy<T>::setStartingPoint(const arma::Col<double> xStart) {
    //TODO: length check
    this->startingPoint = xStart;
  }

  template <typename T>
  void CovarianceMatrixAdaptationEvolutionStrategy<T>::setPopulationSize(const unsigned int popSize) {
    this->populationSize_ = popSize;
  }

  template <typename T>
  void CovarianceMatrixAdaptationEvolutionStrategy<T>::setSingleIteration(const bool DoSingleIteration) {
    this->singleIteration = DoSingleIteration;
  }

  template <typename T>
  std::string CovarianceMatrixAdaptationEvolutionStrategy<T>::toString() const noexcept {
    return "CovarianceMatrixAdaptationEvolutionStrategy";
  }

  template <typename T>
  double CovarianceMatrixAdaptationEvolutionStrategy<T>::getCcov1() const {
    return ccov1;
  }

  template <typename T>
  void CovarianceMatrixAdaptationEvolutionStrategy<T>::setCcov1(double ccov1) {
    this->ccov1 = ccov1;
  }

  template <typename T>
  double CovarianceMatrixAdaptationEvolutionStrategy<T>::getCcovmu() const {
    return ccovmu;
  }

  template <typename T>
  void CovarianceMatrixAdaptationEvolutionStrategy<T>::setCcovmu(double ccovmu) {
    this->ccovmu = ccovmu;
  }

  template <typename T>
  double CovarianceMatrixAdaptationEvolutionStrategy<T>::getCcum() const {
    return ccum;
  }

  template <typename T>
  void CovarianceMatrixAdaptationEvolutionStrategy<T>::setCcum(double ccum) {
    this->ccum = ccum;
  }

  template <typename T>
  double CovarianceMatrixAdaptationEvolutionStrategy<T>::getCs() const {
    return cs;
  }

  template <typename T>
  void CovarianceMatrixAdaptationEvolutionStrategy<T>::setCs(double cs) {
    this->cs = cs;
  }

  template <typename T>
  unsigned int CovarianceMatrixAdaptationEvolutionStrategy<T>::getNumberOfParents() const {
    return numberOfParents;
  }

  template <typename T>
  void CovarianceMatrixAdaptationEvolutionStrategy<T>::setNumberOfParents(unsigned int numberOfParents) {
    this->numberOfParents = numberOfParents;
  }

  template <typename T>
  double CovarianceMatrixAdaptationEvolutionStrategy<T>::getToleranceFun() const {
    return toleranceFun;
  }

  template <typename T>
  void CovarianceMatrixAdaptationEvolutionStrategy<T>::setToleranceFun(double toleranceFun) {
    this->toleranceFun = toleranceFun;
  }

  template <typename T>
  double CovarianceMatrixAdaptationEvolutionStrategy<T>::getToleranceHistFun() const {
    return toleranceHistFun;
  }

  template <typename T>
  void CovarianceMatrixAdaptationEvolutionStrategy<T>::setToleranceHistFun(double toleranceHistFun) {
    this->toleranceHistFun = toleranceHistFun;
  }

  template <typename T>
  double CovarianceMatrixAdaptationEvolutionStrategy<T>::getToleranceUpX() const {
    return toleranceUpX;
  }

  template <typename T>
  void CovarianceMatrixAdaptationEvolutionStrategy<T>::setToleranceUpX(double toleranceUpX) {
    this->toleranceUpX = toleranceUpX;
  }

  template <typename T>
  double CovarianceMatrixAdaptationEvolutionStrategy<T>::getToleranceX() const {
    return toleranceX;
  }

  template <typename T>
  void CovarianceMatrixAdaptationEvolutionStrategy<T>::setToleranceX(double toleranceX) {
    this->toleranceX = toleranceX;
  }

  template <typename T>
  arma::Col<double> CovarianceMatrixAdaptationEvolutionStrategy<T>::getXmean() const {
    return xmean;
  }

  template <typename T>
  void CovarianceMatrixAdaptationEvolutionStrategy<T>::setXmean(arma::Col<double> xmean) {
    this->xmean = xmean;
  }
}