namespace mant {
  template <typename ParameterType, class DistanceFunction>
  class RandomSearch : public SamplingBasedAlgorithm<ParameterType, DistanceFunction> {
    public:
      explicit RandomSearch(
          const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem) noexcept;

      RandomSearch(const RandomSearch&) = delete;
      RandomSearch& operator=(const RandomSearch&) = delete;

      std::string to_string() const noexcept override;

    protected:
      void optimiseImplementation() noexcept override;
  };

  template <typename ParameterType, class DistanceFunction>
  RandomSearch<ParameterType, DistanceFunction>::RandomSearch(
      const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem) noexcept
    : SamplingBasedAlgorithm<ParameterType, DistanceFunction>(optimisationProblem) {

  }

  template <typename ParameterType, class DistanceFunction>
  void RandomSearch<ParameterType, DistanceFunction>::optimiseImplementation() noexcept {
    while(!this->isFinished() && !this->isTerminated()) {
      ++this->numberOfIterations_;

      const arma::Col<ParameterType>& candidateParameter = this->distanceFunction_.getNeighbour(this->optimisationProblem_->getLowerBounds(), this->optimisationProblem_->getUpperBounds() - this->optimisationProblem_->getLowerBounds());
      const double& candidateSoftConstraintsValue = this->optimisationProblem_->getSoftConstraintsValue(candidateParameter);
      const double& candidateObjectiveValue = this->optimisationProblem_->getObjectiveValue(candidateParameter);

      if(candidateSoftConstraintsValue < this->bestSoftConstraintsValue_ || (candidateSoftConstraintsValue == this->bestSoftConstraintsValue_ && candidateObjectiveValue < this->bestObjectiveValue_)) {
        this->bestParameter_ = candidateParameter;
        this->bestSoftConstraintsValue_ = candidateSoftConstraintsValue;
        this->bestObjectiveValue_ = candidateObjectiveValue;
      }
    };
  }

  template <typename ParameterType, class DistanceFunction>
  std::string RandomSearch<ParameterType, DistanceFunction>::to_string() const noexcept {
    return "RandomSearch";
  }
}
