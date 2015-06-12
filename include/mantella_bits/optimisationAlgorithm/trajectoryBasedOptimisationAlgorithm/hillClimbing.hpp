// TODO Add restarting
namespace mant {
  template <typename T = double>
  class HillClimbing : public TrajectoryBasedOptimisationAlgorithm<T> {
    static_assert(std::is_floating_point<T>::value, "The parameter type T must be a floating point type.");
    
    public:
      explicit HillClimbing(
          const std::shared_ptr<OptimisationProblem<T>> optimisationProblem) noexcept;

      void setMaximalStepSize(
          const T maximalStepSize);

      std::string toString() const noexcept override;

    protected:
      T maximalStepSize_;

      void optimiseImplementation() noexcept override;
  };

  //
  // Implementation
  //

  template <typename T>
  HillClimbing<T>::HillClimbing(
      const std::shared_ptr<OptimisationProblem<T>> optimisationProblem) noexcept
    : TrajectoryBasedOptimisationAlgorithm<T>(optimisationProblem) {
    setMaximalStepSize(arma::norm(this->getLowerBounds(), this->getUpperBounds()) / static_cast<T>(10.0L));
  }

  template <typename T>
  void HillClimbing<T>::optimiseImplementation() noexcept {
    ++this->numberOfIterations_;

    this->bestParameter_ = this->initialParameter_;
    this->bestSoftConstraintsValue_ = this->getSoftConstraintsValue(this->initialParameter_);
    this->bestObjectiveValue_ = this->getObjectiveValue(this->initialParameter_);

    while(!this->isFinished() && !this->isTerminated()) {
      ++this->numberOfIterations_;

      const arma::Col<T>& candidateParameter = this->getRandomNeighbour(this->bestParameter_, static_cast<T>(0.0L), maximalStepSize_);

      updateBestParameter(candidateParameter, this->getSoftConstraintsValue(candidateParameter), this->getObjectiveValue(candidateParameter));
    }
  }

  template <typename T>
  void HillClimbing<T>::setMaximalStepSize(
      const T maximalStepSize) {
    if (maximalStepSize <= 0) {
      throw std::logic_error("The maximal step size must be strict greater than 0.");
    }

    maximalStepSize_ = maximalStepSize;
  }

  template <typename T>
  std::string HillClimbing<T>::toString() const noexcept {
    return "HillClimbing";
  }
}
