// TODO Add restarting
namespace mant {
  template <typename T>
  class SimulatedAnnealing : public TrajectoryBasedOptimisationAlgorithm<T> {
    public:
      explicit SimulatedAnnealing(
          const std::shared_ptr<OptimisationProblem<T>> optimisationProblem) noexcept;

      void setMaximalStepSize(
          const T maximalStepSize);

      std::string toString() const noexcept override;

    protected:
      T maximalStepSize_;

      virtual bool isAcceptableState(
          const double candidateObjectiveValue) noexcept;

      void optimiseImplementation() noexcept override;

      void setDefaultMaximalStepSize(std::true_type) noexcept;
      void setDefaultMaximalStepSize(std::false_type) noexcept;
  };

  //
  // Implementation
  //

  template <typename T>
  SimulatedAnnealing<T>::SimulatedAnnealing(
      const std::shared_ptr<OptimisationProblem<T>> optimisationProblem) noexcept
    : TrajectoryBasedOptimisationAlgorithm<T>(optimisationProblem) {
    setDefaultMaximalStepSize(std::is_floating_point<T>());
  }

  template <typename T>
  void SimulatedAnnealing<T>::optimiseImplementation() noexcept {
    ++this->numberOfIterations_;

    this->bestParameter_ = this->initialParameter_;
    this->bestSoftConstraintsValue_ = this->getSoftConstraintsValue(this->initialParameter_);
    this->bestObjectiveValue_ = this->getObjectiveValue(this->initialParameter_);

    arma::Col<double> state = this->bestParameter_;
    while(!this->isFinished() && !this->isTerminated()) {
      ++this->numberOfIterations_;

      arma::Col<T> candidateParameter = this->distanceFunction_->getRandomNeighbour(state, 0, maximalStepSize_);

      const arma::Col<unsigned int>& belowLowerBound = arma::find(candidateParameter < this->getLowerBounds());
      const arma::Col<unsigned int>& aboveUpperBound = arma::find(candidateParameter > this->getUpperBounds());

      candidateParameter.elem(belowLowerBound) = this->getLowerBounds().elem(belowLowerBound);
      candidateParameter.elem(aboveUpperBound) = this->getUpperBounds().elem(aboveUpperBound);

      const double& candidateSoftConstraintsValue = this->getSoftConstraintsValue(candidateParameter);
      const double& candidateObjectiveValue = this->getObjectiveValue(candidateParameter);

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

  template <typename T>
  bool SimulatedAnnealing<T>::isAcceptableState(
      const double candidateObjectiveValue) noexcept {
    return std::exp((this->bestObjectiveValue_ - candidateObjectiveValue) / (this->numberOfIterations_ / this->maximalNumberOfIterations_)) < std::uniform_real_distribution<double>(0.0, 1.0)(Rng::getGenerator());
  }

  template <typename T>
  void SimulatedAnnealing<T>::setMaximalStepSize(
      const T maximalStepSize) {
    if (maximalStepSize <= 0) {
      throw std::logic_error("The maximal step size must be strict greater than 0.");
    }

    maximalStepSize_ = maximalStepSize;
  }

  template <typename T>
  std::string SimulatedAnnealing<T>::toString() const noexcept {
    return "SimulatedAnnealing";
  }

  template <typename T>
  void SimulatedAnnealing<T>::setDefaultMaximalStepSize(
      std::true_type) noexcept {
    setMaximalStepSize(this->distanceFunction_->getDistance(this->getLowerBounds(), this->getUpperBounds()) / 10.0);
  }

  template <typename T>
  void SimulatedAnnealing<T>::setDefaultMaximalStepSize(
      std::false_type) noexcept {
    setMaximalStepSize(arma::max(1, this->distanceFunction_->getDistance(this->getLowerBounds(), this->getUpperBounds()) / 10));
  }
}
