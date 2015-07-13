namespace mant {

  template <typename T>
  class CovarianceMatrixAdaptationEvolutionStrategy : public PopulationBasedOptimisationAlgorithm<T> {
  public:
    explicit CovarianceMatrixAdaptationEvolutionStrategy(
        const std::shared_ptr<OptimisationProblem<T>> optimisationProblem,
        const unsigned int populationSize) noexcept;

    unsigned int getIRun();
    void setIRun(const unsigned int irun); //irun
    void setLambda0(const double lambda0); //lambda0
    void setStepSize(const arma::Col<double> sigma); //insigma
    void setStartingPoint(const arma::Col<double> xStart); //xstart
    
    void setSingleIteration(const bool DoSingleIteration);

    std::string toString() const noexcept override;

  protected:
    //Notation: comments always start with matlab name of variable
    //          comments after ';' are from matlab code
    //          comments after '-' are from mantella

    arma::Col<double> startingPoint; //definput.xstart
    arma::Col<double> stepSize; //definput.insigma

    bool stopOnStagnation = true; //defopts.StopOnStagnation; stop when fitness stagnates for a long time
    bool stopOnWarnings = true; //defopts.StopOnWarnings
    arma::Col<double> lowerBounds; //defopts.LBounds
    arma::Col<double> upperBounds; //defopts.UBounds
    bool evalParallel = true; //defopts.EvalParallel; objective function FUN accepts NxM matrix, with M>1?
    unsigned int restarts = 9; //defopts.Restarts - HCMA default is 9
    double incPopSize; //defopts.IncPopSize; multiplier for population size before each restart
    unsigned int numberOfParents; //defopts.ParentNumber
    double cs; //defopts.CMA.cs; cumulation constant for step-size
    double damping; //defopts.CMA.damps; damping for step-size
    double ccum; //defopts.CMA.ccum; 
    double ccov1; //defopts.CMA.ccov1;
    double ccovmu; //defopts.CMA.ccovmu;
    unsigned int activeCMA; ////defopts.CMA.active; active CMA 1: neg. updates with pos. def. check, 2: neg. updates
    unsigned int irun = 0; //irun

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
    
    //HCMA needs to be able to do single iterations of CMAES, for that this bool can be set.
    bool singleIteration = false;

    void optimiseImplementation() override;
  };

  template <typename T>
  CovarianceMatrixAdaptationEvolutionStrategy<T>::CovarianceMatrixAdaptationEvolutionStrategy(const std::shared_ptr<OptimisationProblem<T>> optimisationProblem,
      const unsigned int populationSize) noexcept
  : PopulationBasedOptimisationAlgorithm<T>(optimisationProblem, populationSize) {
    //TODO: ccov1,ccovmu,ccum get overriden in xacmes
    //TODO: Tol** variables get overridden - are they needed?

    //overwritten by constructor value
    //populationSize = 4 + std::floor(3*std::log(numberOfDimensions_));
    numberOfParents = populationSize / 2;

    //HCMA settings coming from xacmes.m
    setStartingPoint(-4 + 8 * arma::randu(this->numberOfDimensions_));
    setStepSize(arma::Col<double>{2.0});

    xmean = arma::Col<double>(this->numberOfDimensions_);
    lowerBounds = arma::Col<double>(this->numberOfDimensions_);
    lowerBounds.fill(-arma::datum::inf);
    upperBounds = arma::Col<double>(this->numberOfDimensions_);
    upperBounds.fill(arma::datum::inf);

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
    
    //TODO: cmaes_initializeRun checks boundaries here, but our boundaries are always -inf to inf?
  }

  template <typename T>
  void CovarianceMatrixAdaptationEvolutionStrategy<T>::optimiseImplementation() {
    //TODO: figure out what to do with lambda0 and popsize settings
    //-this overrides the constructor value,also it gets split into lambda and popsize,
    //but neither variable is written to ever again
    //xacmes also writes these variables again...
    lambda0 = std::floor(this->populationSize_ * std::pow(incPopSize, irun - 1));
    this->populationSize_ = lambda0;
    
    bool stopFlag = false;
    while(!stopFlag) {
      
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
  void CovarianceMatrixAdaptationEvolutionStrategy<T>::setSingleIteration(const bool DoSingleIteration) {
    this->singleIteration = DoSingleIteration;
  }

  template <typename T>
  std::string CovarianceMatrixAdaptationEvolutionStrategy<T>::toString() const noexcept {
    return "CovarianceMatrixAdaptationEvolutionStrategy";
  }
}