namespace mant {
  template <typename T = double>
  class TrajectoryBasedOptimisationAlgorithm : public OptimisationAlgorithm<T> {
    static_assert(std::is_floating_point<T>::value, "The parameter type T must be a floating point type.");
  
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
    setInitialParameter(arma::randu<arma::Col<T>>(this->numberOfDimensions_) % (this->getUpperBounds() - this->getLowerBounds()) + this->getLowerBounds());
  }

  template <typename T>
  void TrajectoryBasedOptimisationAlgorithm<T>::setInitialParameter(
      const arma::Col<T> initialParameter) {
    verify(initialParameter.n_elem == this->numberOfDimensions_, "The number of dimensions of the initial parameter must match the number of dimensions of the optimisation problem.");

    initialParameter_ = initialParameter;
  }
}
