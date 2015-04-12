namespace mant {

  template <typename ParameterType>
  class SelectTheEasiestPoint : public SamplingBasedOptimisationAlgorithm<ParameterType> {
  public:
    explicit SelectTheEasiestPoint(
        const std::shared_ptr<OptimisationProblem<double>> optimisationProblem, const unsigned int solvingDimension) noexcept;

    std::string toString() const noexcept override;

    void setSolvingDimension(const unsigned int solvingDimension);

  protected:
    void optimiseImplementation() noexcept override;

    double lowerBound; //TODO: HCMA sets this to -5 in all cases... (xacmes.m 90)
    double upperBound; //TODO: HCMA sets this to 5 in all cases... (xacmes.m 90)
    unsigned int solvingDimension; //ix in HCMA. gets set to i-1 in mystepinit, than incremented by 1 in mystep initialization...
    arma::Col<double> sol;
    unsigned int parameterCount = 1;
    arma::Col<double> parameterHistory;
    arma::Col<double> functionValueHistory;
  };

  //
  // Implementation
  //

  //Be aware that STEP already uses two function evaluations and thous numberOfIterations in the constructor!
  //if a result is found, the specific value for the solvingDimension can be found in bestParameter(solvingDimension)

  template <typename ParameterType>
  SelectTheEasiestPoint<ParameterType>::SelectTheEasiestPoint(
      const std::shared_ptr<OptimisationProblem<double>> optimisationProblem, const unsigned int solvingDimension) noexcept
  : SamplingBasedOptimisationAlgorithm<ParameterType>(optimisationProblem) {
    std::cout << "we have lift off!" << std::endl;
    setSolvingDimension(solvingDimension);

    sol = arma::Col<double>(this->numberOfDimensions_);
    parameterHistory = arma::Col<double>(this->numberOfIterations_); //NOI should suffice as size, 
    functionValueHistory = arma::Col<double>(this->numberOfIterations_);

    lowerBound = optimisationProblem->getLowerBounds()(solvingDimension);
    upperBound = optimisationProblem->getUpperBounds()(solvingDimension);

    sol = (lowerBound + (upperBound - lowerBound)) * arma::randu(this->numberOfDimensions_); //mySTEPinit 18

    parameterHistory(0) = lowerBound;
    sol(solvingDimension) = lowerBound;
    functionValueHistory(0) = -this->getObjectiveValue(sol);
    this->numberOfIterations_++;
    //this never gets done in "normal" STEP, but we need to do it
    if (arma::max(functionValueHistory) < this->bestObjectiveValue_) {
      this->bestObjectiveValue_ = arma::max(functionValueHistory);
      this->bestParameter_ = sol;
    }

    parameterHistory(1) = upperBound;
    sol(solvingDimension) = upperBound;
    functionValueHistory(1) = -this->getObjectiveValue(sol);
    this->numberOfIterations_++;
    //this never gets done in "normal" STEP, but we need to do it
    if (arma::max(functionValueHistory) < this->bestObjectiveValue_) {
      this->bestObjectiveValue_ = arma::max(functionValueHistory);
      this->bestParameter_ = sol;
    }
  }

  template <typename ParameterType>
  void SelectTheEasiestPoint<ParameterType>::optimiseImplementation() noexcept {
    unsigned int i1 = 0; //i1 and i2 are both used as indexes so their value is 1 less than in matlab
    unsigned int i2 = 0;

    while (!this->isFinished() & !this->isTerminated()) {
      double ftarget = arma::max(functionValueHistory);
      if (parameterCount == 1) {
        i1 = 0;
        i2 = 1;
      } else {
        //arma::Col<double> dx = t.rows(1, nt) - t.rows(0, nt - 1);
        arma::Col<double> dy = functionValueHistory.rows(1, parameterCount) - functionValueHistory.rows(0, parameterCount - 1);
        arma::Col<double> yhat = ftarget - functionValueHistory.rows(0, parameterCount - 1); //TODO: t/feps handling!!
        arma::Col<double> D = 4 * yhat - 2 * dy + 4 * arma::sqrt(yhat % yhat - yhat % dy); //dividing everything by dx² omitted since pointless.
        i1 = arma::sort_index(D)(0);
        i2 = i1 + 1;
      }
      parameterCount++;
      
      //may have to enlargen vectors
      if (parameterCount >= parameterHistory.n_elem) {
        parameterHistory.resize(parameterHistory.n_elem + 100);
        functionValueHistory.resize(functionValueHistory.n_elem + 100);
      }

      //setting up and calcing next vector
      parameterHistory(parameterCount) = parameterHistory(i1) + 0.5 * (parameterHistory(i2) - parameterHistory(i1));
      sol(solvingDimension) = parameterHistory(parameterCount);
      functionValueHistory(parameterCount) = -this->getObjectiveValue(sol);
      this->numberOfIterations_++;
      //this never gets done in "normal" STEP, but we need to do it
      if (arma::max(functionValueHistory) < this->bestObjectiveValue_) {
        this->bestObjectiveValue_ = arma::max(functionValueHistory);
        this->bestParameter_ = sol;
      }

      //parameters (and thous function values) have to be sorted so dy calculation works in next iteration
      arma::uvec sortIndexT = arma::sort_index(parameterHistory);
      parameterHistory = arma::sort(parameterHistory);
      functionValueHistory = functionValueHistory.rows(sortIndexT);


    }
  }

  template <typename ParameterType>
  void SelectTheEasiestPoint<ParameterType>::setSolvingDimension(const unsigned int solvingDimension) {
    this->solvingDimension = solvingDimension;
  }

  template <typename ParameterType>
  std::string SelectTheEasiestPoint<ParameterType>::toString() const noexcept {
    return "SelectTheEasiestPoint";
  }
}

