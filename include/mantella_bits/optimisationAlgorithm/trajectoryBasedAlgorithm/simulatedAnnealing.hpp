// TODO Add restarting
namespace mant {
  template <typename ParameterType, class DistanceFunction>
  class SimulatedAnnealing : public TrajectoryBasedAlgorithm<ParameterType, DistanceFunction> {
    public:
      explicit SimulatedAnnealing(
          const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem) noexcept;

      SimulatedAnnealing(const SimulatedAnnealing&) = delete;
      SimulatedAnnealing& operator=(const SimulatedAnnealing&) = delete;

      void setMaximalStepSize(
          const ParameterType& maximalStepSize);

      std::string to_string() const noexcept override;

    protected:
      ParameterType maximalStepSize_;

      virtual bool isAcceptableState(
          const double& candidateObjectiveValue) noexcept;

      void optimiseImplementation() noexcept override;

      void setDefaultMaximalStepSize(std::true_type) noexcept;
      void setDefaultMaximalStepSize(std::false_type) noexcept;
  };

  template <typename ParameterType, class DistanceFunction>
  SimulatedAnnealing<ParameterType, DistanceFunction>::SimulatedAnnealing(
      const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem) noexcept
    : TrajectoryBasedAlgorithm<ParameterType, DistanceFunction>(optimisationProblem) {
    setDefaultMaximalStepSize(std::is_floating_point<ParameterType>());
  }

  template <typename ParameterType, class DistanceFunction>
  void SimulatedAnnealing<ParameterType, DistanceFunction>::optimiseImplementation() noexcept {
    ++this->numberOfIterations_;

    this->bestParameter_ = this->initialParameter_;
    this->bestSoftConstraintsValue_ = this->optimisationProblem_->getSoftConstraintsValue(this->initialParameter_);
    this->bestObjectiveValue_ = this->optimisationProblem_->getObjectiveValue(this->initialParameter_);

    arma::Col<double> state = this->bestParameter_;
    while(!this->isFinished() && !this->isTerminated()) {
      ++this->numberOfIterations_;

      arma::Col<ParameterType> candidateParameter = this->distanceFunction_.getNeighbour(state, 0, maximalStepSize_);

      const arma::Col<unsigned int>& belowLowerBound = arma::find(candidateParameter < this->optimisationProblem_->getLowerBounds());
      const arma::Col<unsigned int>& aboveUpperBound = arma::find(candidateParameter > this->optimisationProblem_->getUpperBounds());

      candidateParameter.elem(belowLowerBound) = this->optimisationProblem_->getLowerBounds().elem(belowLowerBound);
      candidateParameter.elem(aboveUpperBound) = this->optimisationProblem_->getUpperBounds().elem(aboveUpperBound);

      const double& candidateSoftConstraintsValue = this->optimisationProblem_->getSoftConstraintsValue(candidateParameter);
      const double& candidateObjectiveValue = this->optimisationProblem_->getObjectiveValue(candidateParameter);

      if(candidateSoftConstraintsValue < this->bestSoftConstraintsValue_ || (candidateSoftConstraintsValue == this->bestSoftConstraintsValue_ && candidateObjectiveValue < this->bestObjectiveValue_)) {
        state = candidateParameter;

        this->bestParameter_ = candidateParameter;
        this->bestSoftConstraintsValue_ = candidateSoftConstraintsValue;
        this->bestObjectiveValue_ = candidateObjectiveValue;
      } else if(isAcceptableState(candidateObjectiveValue)) {
        state = candidateParameter;
      }
    }
  }

  template <typename ParameterType, class DistanceFunction>
  bool SimulatedAnnealing<ParameterType, DistanceFunction>::isAcceptableState(
      const double& candidateObjectiveValue) noexcept {
    return std::exp((this->bestObjectiveValue_ - candidateObjectiveValue) / (this->numberOfIterations_ / this->maximalNumberOfIterations_)) < std::uniform_real_distribution<double>(0.0, 1.0)(Rng::getGenerator());
  }

  template <typename ParameterType, class DistanceFunction>
  void SimulatedAnnealing<ParameterType, DistanceFunction>::setMaximalStepSize(
      const ParameterType& maximalStepSize) {
    if (maximalStepSize <= 0) {
      throw std::logic_error("The maximal step size must be strict greater than 0.");
    }

    maximalStepSize_ = maximalStepSize;
  }

  template <typename ParameterType, class DistanceFunction>
  std::string SimulatedAnnealing<ParameterType, DistanceFunction>::to_string() const noexcept {
    return "SimulatedAnnealing";
  }

  template <typename ParameterType, class DistanceFunction>
  void SimulatedAnnealing<ParameterType, DistanceFunction>::setDefaultMaximalStepSize(
      std::true_type) noexcept {
    setMaximalStepSize(this->distanceFunction_.getDistance(this->optimisationProblem_->getLowerBounds(), this->optimisationProblem_->getUpperBounds()) / 10.0);
  }

  template <typename ParameterType, class DistanceFunction>
  void SimulatedAnnealing<ParameterType, DistanceFunction>::setDefaultMaximalStepSize(
      std::false_type) noexcept {
    setMaximalStepSize(arma::max(1, this->distanceFunction_.getDistance(this->optimisationProblem_->getLowerBounds(), this->optimisationProblem_->getUpperBounds()) / 10));
  }
}
