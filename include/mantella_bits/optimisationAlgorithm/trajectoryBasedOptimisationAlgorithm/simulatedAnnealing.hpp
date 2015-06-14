// TODO Add restarting
namespace mant {
  template <typename T = double>
  class SimulatedAnnealing : public TrajectoryBasedOptimisationAlgorithm<T> {
    static_assert(std::is_floating_point<T>::value, "The parameter type T must be a floating point type.");
    
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
  };

  //
  // Implementation
  //

  template <typename T>
  SimulatedAnnealing<T>::SimulatedAnnealing(
      const std::shared_ptr<OptimisationProblem<T>> optimisationProblem) noexcept
    : TrajectoryBasedOptimisationAlgorithm<T>(optimisationProblem) {
    setMaximalStepSize(arma::norm(this->getLowerBounds(), this->getUpperBounds()) / static_cast<T>(10.0L));
  }

  template <typename T>
  void SimulatedAnnealing<T>::optimiseImplementation() noexcept {
    ++this->numberOfIterations_;

    this->bestParameter_ = this->initialParameter_;
    this->bestSoftConstraintsValue_ = this->getSoftConstraintsValue(this->initialParameter_);
    this->bestObjectiveValue_ = this->getObjectiveValue(this->initialParameter_);

    arma::Col<T> state = this->bestParameter_;
    while(!this->isFinished() && !this->isTerminated()) {
      ++this->numberOfIterations_;

      const arma::Col<T>& candidateParameter = this->getRandomNeighbour(state, static_cast<T>(0.0L), maximalStepSize_);

      const double& candidateSoftConstraintsValue = this->getSoftConstraintsValue(candidateParameter);
      const double& candidateObjectiveValue = this->getObjectiveValue(candidateParameter);

      if (updateBestParameter(candidateParameter, candidateSoftConstraintsValue, candidateObjectiveValue) || isAcceptableState(candidateSoftConstraintsValue + candidateObjectiveValue)) {
        state = candidateParameter;
      }
    }
  }

  template <typename T>
  bool SimulatedAnnealing<T>::isAcceptableState(
      const double candidateObjectiveValue) noexcept {
    return std::exp((this->bestObjectiveValue_ - candidateObjectiveValue) / static_cast<double>(this->numberOfIterations_ / this->maximalNumberOfIterations_)) < std::uniform_real_distribution<double>(0.0, 1.0)(Rng::getGenerator());
  }

  template <typename T>
  void SimulatedAnnealing<T>::setMaximalStepSize(
      const T maximalStepSize) {
    verify(maximalStepSize > static_cast<T>(0.0L), "The maximal step size must be strict greater than 0.");

    maximalStepSize_ = maximalStepSize;
  }

  template <typename T>
  std::string SimulatedAnnealing<T>::toString() const noexcept {
    return "simulated_annealing";
  }
}
