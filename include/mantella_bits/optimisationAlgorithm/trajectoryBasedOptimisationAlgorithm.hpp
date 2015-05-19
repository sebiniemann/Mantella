namespace mant {
  template <typename T>
  class TrajectoryBasedOptimisationAlgorithm : public OptimisationAlgorithm<T> {
    public:
      explicit TrajectoryBasedOptimisationAlgorithm(
          const std::shared_ptr<OptimisationProblem<T>> optimisationProblem) noexcept;

      void setInitialParameter(
          const arma::Col<T> initialParameter);

    protected:
      arma::Col<T> initialParameter_;
  };

  //
  // Implementation
  //

  template <typename T>
  TrajectoryBasedOptimisationAlgorithm<T>::TrajectoryBasedOptimisationAlgorithm(
      const std::shared_ptr<OptimisationProblem<T>> optimisationProblem) noexcept
    : OptimisationAlgorithm<T>(optimisationProblem) {
    // TODO fix for unsigned int
    setInitialParameter(arma::randu<arma::Col<T>>(this->numberOfDimensions_) % (this->getUpperBounds() - this->getLowerBounds()) + this->getLowerBounds());
  }

  template <typename T>
  void TrajectoryBasedOptimisationAlgorithm<T>::setInitialParameter(
      const arma::Col<T> initialParameter) {
    if(initialParameter.n_elem != this->numberOfDimensions_) {
      throw std::logic_error("The number of dimensions of the initial parameter (" + std::to_string(initialParameter.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(this->numberOfDimensions_) + ").");
    }

    initialParameter_ = initialParameter;
  }
}
