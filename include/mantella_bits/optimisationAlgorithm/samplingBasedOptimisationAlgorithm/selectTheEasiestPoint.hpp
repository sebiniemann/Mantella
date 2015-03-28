namespace mant {

  template <typename ParameterType>
  class selectTheEasiestPoint : public SamplingBasedOptimisationAlgorithm<ParameterType> {
  public:
    explicit selectTheEasiestPoint(
        const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem, unsigned int solvingDimension) noexcept;

    std::string toString() const noexcept override;

    void setSolvingDimension(const unsigned int solvingDimension);

  protected:
    void optimiseImplementation() noexcept override;

    double lowerBound; //TODO: HCMA sets this to -5 in all cases... (xacmes.m 90)
    double upperBound; //TODO: HCMA sets this to 5 in all cases... (xacmes.m 90)
    unsigned int solvingDimension; //ix in matlab. gets set to i-1 in mystepinit, than incremented by 1 in mystep initialization...
    arma::Col<double> sol;
    unsigned int nt = 1;
    arma::Col<double> t;
    arma::Col<double> ft;
  };

  //
  // Implementation
  //

  //Be aware that STEP already uses two function evaluations and thous numberOfIterations in the constructor!
  //if a result is found, the specific value for the solvingDimension can be found in bestParameter(solvingDimension)

  template <typename ParameterType>
  selectTheEasiestPoint<ParameterType>::selectTheEasiestPoint(
      const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem, unsigned int solvingDimension) noexcept
  : SamplingBasedOptimisationAlgorithm<ParameterType>(optimisationProblem) {
    setSolvingDimension(solvingDimension);

    sol = arma::Col<double>(this->numberOfDimensions_);
    t = arma::Col<double>(200);
    ft = arma::Col<double>(200);

    lowerBound = optimisationProblem->getLowerBounds()(solvingDimension);
    upperBound = optimisationProblem->getUpperBounds()(solvingDimension);

    sol = (lowerBound + (upperBound - lowerBound)) * arma::randu(this->numberOfDimensions_); //mySTEPinit 18

    t(0) = lowerBound;
    sol(solvingDimension) = lowerBound;
    ft(0) = -this->getObjectiveValue(sol);
    this->numberOfIterations_++;
    //this never gets done in "normal" STEP, but we need to do it
    if (arma::max(ft) < this->bestObjectiveValue_) {
      this->bestObjectiveValue_ = arma::max(ft);
      this->bestParameter_ = sol;
    }

    t(1) = upperBound;
    sol(solvingDimension) = upperBound;
    ft(1) = -this->getObjectiveValue(sol);
    this->numberOfIterations_++;
    //this never gets done in "normal" STEP, but we need to do it
    if (arma::max(ft) < this->bestObjectiveValue_) {
      this->bestObjectiveValue_ = arma::max(ft);
      this->bestParameter_ = sol;
    }
  }

  template <typename ParameterType>
  void selectTheEasiestPoint<ParameterType>::optimiseImplementation() noexcept {
    while (!this->isFinished() & !this->isTerminated()) {

      unsigned int i1 = 0; //i1 and i2 are both used and indexes so their value is 1 less than in matlab
      unsigned int i2 = 0;

      double ftarget = arma::max(ft);
      if (nt == 1) {
        i1 = 0;
        i2 = 1;
      } else {
        //arma::Col<double> dx = t.rows(1, nt) - t.rows(0, nt - 1);
        arma::Col<double> dy = ft.rows(1, nt) - ft.rows(0, nt - 1);
        arma::Col<double> yhat = ftarget - ft.rows(0, nt - 1); //TODO: t/feps handling!!
        arma::Col<double> D = (4 * yhat - 2 * dy + 4 * arma::sqrt(yhat % yhat - yhat % dy)); //dividing everything by dx² omitted since pointless.
        i1 = arma::sort_index(D)(0);
        i2 = i1 + 1;
      }
      nt++;
      //may have to enlargen vectors
      if (nt >= t.n_elem) {
        t.resize(t.n_elem + 100);
        ft.resize(ft.n_elem + 100);
      }

      t(nt) = t(i1) + 0.5 * (t(i2) - t(i1));
      sol(solvingDimension) = t(nt);
      ft(nt) = -this->getObjectiveValue(sol);
      this->numberOfIterations_++;
      //this never gets done in "normal" STEP, but we need to do it
      if (arma::max(ft) < this->bestObjectiveValue_) {
        this->bestObjectiveValue_ = arma::max(ft);
        this->bestParameter_ = sol;
      }

      arma::uvec sortIndexT = arma::sort_index(t);
      t = arma::sort(t);
      ft = ft.rows(sortIndexT);


    }
  }

  template <typename ParameterType>
  void selectTheEasiestPoint<ParameterType>::setSolvingDimension(const unsigned int solvingDimension) {
    this->solvingDimension = solvingDimension;
  }

  template <typename ParameterType>
  std::string selectTheEasiestPoint<ParameterType>::toString() const noexcept {
    return "selectTheEasiestPoint";
  }
}

