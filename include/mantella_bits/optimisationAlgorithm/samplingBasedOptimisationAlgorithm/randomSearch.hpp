namespace mant {
  template <typename T>
  class RandomSearch : public SamplingBasedOptimisationAlgorithm<T> {
    public:
      explicit RandomSearch(
          const std::shared_ptr<OptimisationProblem<T>> optimisationProblem) noexcept;

      std::string toString() const noexcept override;

    protected:
      void optimiseImplementation() noexcept override;
  };

  //
  // Implementation
  //

  template <typename T>
  RandomSearch<T>::RandomSearch(
      const std::shared_ptr<OptimisationProblem<T>> optimisationProblem) noexcept
    : SamplingBasedOptimisationAlgorithm<T>(optimisationProblem) {

  }

  template <typename T>
  void RandomSearch<T>::optimiseImplementation() noexcept {
    while(!this->isFinished() && !this->isTerminated()) {
      ++this->numberOfIterations_;

      const arma::Col<T>& candidateParameter = this->distanceFunction_->getRandomNeighbour(arma::zeros<arma::Col<double>>(numberOfDimensions_), this->getLowerBounds(), this->getUpperBounds() - this->getLowerBounds());
      const double& candidateSoftConstraintsValue = this->getSoftConstraintsValue(candidateParameter);
      const double& candidateObjectiveValue = this->getObjectiveValue(candidateParameter);

      if(candidateSoftConstraintsValue < this->bestSoftConstraintsValue_ || (candidateSoftConstraintsValue == this->bestSoftConstraintsValue_ && candidateObjectiveValue < this->bestObjectiveValue_)) {
        this->bestParameter_ = candidateParameter;
        this->bestSoftConstraintsValue_ = candidateSoftConstraintsValue;
        this->bestObjectiveValue_ = candidateObjectiveValue;
      }
    };
  }

  template <typename T>
  std::string RandomSearch<T>::toString() const noexcept {
    return "RandomSearch";
  }
}
