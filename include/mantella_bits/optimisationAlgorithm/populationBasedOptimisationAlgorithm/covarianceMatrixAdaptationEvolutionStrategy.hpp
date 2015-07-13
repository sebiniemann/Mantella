namespace mant {

  template <typename T>
  class CovarianceMatrixAdaptationEvolutionStrategy : public PopulationBasedOptimisationAlgorithm<T> {
  public:
    explicit CovarianceMatrixAdaptationEvolutionStrategy(
        const std::shared_ptr<OptimisationProblem<T>> optimisationProblem,
        const unsigned int populationSize) noexcept;
    
    void setStepSize(const arma::Col<double> sigma); //insigma
    void setStartingPoint(const arma::Col<double> xStart); //xstart

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
    uint restarts = 9; //defopts.Restarts - HCMA default is 9
    uint numberOfParents; //defopts.ParentNumber
    uint diagonalOnly; //defopts.DiagonalOnly; C is diagonal for given iterations, 1==always
    double cs; //defopts.CMA.cs; cumulation constant for step-size
    double damping; //defopts.CMA.damps; damping for step-size
    double ccum; //defopts.CMA.ccum; 
    double ccov1; //defopts.CMA.ccov1;
    double ccovmu; //defopts.CMA.ccovmu;
    uint activeCMA; ////defopts.CMA.active; active CMA 1: neg. updates with pos. def. check, 2: neg. updates

    void optimiseImplementation() override;
  };
  
  template <typename T>
  CovarianceMatrixAdaptationEvolutionStrategy<T>::CovarianceMatrixAdaptationEvolutionStrategy(const std::shared_ptr<OptimisationProblem<T>> optimisationProblem,
      const unsigned int populationSize) noexcept
    : PopulationBasedOptimisationAlgorithm<T>(optimisationProblem, populationSize) {
    //TODO: ccov1,ccovmu,ccum get overriden in xacmes
    //TODO: Tol** variables get overridden - are they needed?
  }
  
  template <typename T>
  void CovarianceMatrixAdaptationEvolutionStrategy<T>::optimiseImplementation() {
    
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
  std::string CovarianceMatrixAdaptationEvolutionStrategy<T>::toString() const noexcept {
    return "CovarianceMatrixAdaptationEvolutionStrategy";
  }
}