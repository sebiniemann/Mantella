// TODO Add restarting
namespace mant {
  template <typename T>
  class HillClimbing : public TrajectoryBasedOptimisationAlgorithm<T> {
    public:
      explicit HillClimbing(
          const std::shared_ptr<OptimisationProblem<T>> optimisationProblem) noexcept;

      void setMaximalStepSize(
          const T maximalStepSize);

      std::string toString() const noexcept override;

    protected:
      T maximalStepSize_;

      void optimiseImplementation() noexcept override;

      void setDefaultMaximalStepSize(std::true_type) noexcept;
      void setDefaultMaximalStepSize(std::false_type) noexcept;
  };

  //
  // Implementation
  //

  template <typename T>
  HillClimbing<T>::HillClimbing(
      const std::shared_ptr<OptimisationProblem<T>> optimisationProblem) noexcept
    : TrajectoryBasedOptimisationAlgorithm<T>(optimisationProblem) {
    setDefaultMaximalStepSize(std::is_floating_point<T>());
  }

  template <typename T>
  void HillClimbing<T>::optimiseImplementation() noexcept {
    ++this->numberOfIterations_;

    this->bestParameter_ = this->initialParameter_;
    this->bestSoftConstraintsValue_ = this->getSoftConstraintsValue(this->initialParameter_);
    this->bestObjectiveValue_ = this->getObjectiveValue(this->initialParameter_);

    while(!this->isFinished() && !this->isTerminated()) {
      ++this->numberOfIterations_;

      arma::Col<T> candidateParameter = this->distanceFunction_->getRandomNeighbour(this->bestParameter_, 0, maximalStepSize_);

      const arma::Col<unsigned int>& belowLowerBound = arma::find(candidateParameter < this->getLowerBounds());
      const arma::Col<unsigned int>& aboveUpperBound = arma::find(candidateParameter > this->getUpperBounds());

      candidateParameter.elem(belowLowerBound) = this->getLowerBounds().elem(belowLowerBound);
      candidateParameter.elem(aboveUpperBound) = this->getUpperBounds().elem(aboveUpperBound);

      const double& candidateSoftConstraintsValue = this->getSoftConstraintsValue(candidateParameter);
      const double& candidateObjectiveValue = this->getObjectiveValue(candidateParameter);

      if(candidateSoftConstraintsValue < this->bestSoftConstraintsValue_ || (candidateSoftConstraintsValue == this->bestSoftConstraintsValue_ && candidateObjectiveValue < this->bestObjectiveValue_)) {
        this->bestParameter_ = candidateParameter;
        this->bestSoftConstraintsValue_ = candidateSoftConstraintsValue;
        this->bestObjectiveValue_ = candidateObjectiveValue;
      }
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

  template <typename T>
  void HillClimbing<T>::setDefaultMaximalStepSize(
      std::true_type) noexcept {
    setMaximalStepSize(this->distanceFunction_->getDistance(this->getLowerBounds(), this->getUpperBounds()) / 10.0);
  }

  template <typename T>
  void HillClimbing<T>::setDefaultMaximalStepSize(
      std::false_type) noexcept {
    setMaximalStepSize(arma::max(1, this->distanceFunction_->getDistance(this->getLowerBounds(), this->getUpperBounds()) / 10));
  }
}
