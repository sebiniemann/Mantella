namespace mant {
  template <typename T, typename U = double>
  class TrajectoryBasedOptimisationAlgorithm : public OptimisationAlgorithm<T, U> {
    static_assert(arma::is_supported_elem_type<T>::value, "The parameter type T must be supported by Armadillo C++, i.e. an arithmetic types, except bool.");
    static_assert(std::is_arithmetic<U>::value, "U must be an arithmetic type.");
  
    public:
      template <typename V = T, typename std::enable_if<std::is_floating_point<V>::value, bool>::type = false>
      explicit TrajectoryBasedOptimisationAlgorithm(
          const std::shared_ptr<OptimisationProblem<T, U>> optimisationProblem) noexcept;
          
      template <typename V = T, typename std::enable_if<std::is_integral<V>::value, bool>::type = false>
      explicit TrajectoryBasedOptimisationAlgorithm(
          const std::shared_ptr<OptimisationProblem<T, U>> optimisationProblem) noexcept;

      void setInitialParameter(
          const arma::Col<T> initialParameter);

    protected:
      arma::Col<T> initialParameter_;
  };

  //
  // Implementation
  //

  template <typename T, typename U>
  template <typename V, typename std::enable_if<std::is_floating_point<V>::value, bool>::type>
  TrajectoryBasedOptimisationAlgorithm<T, U>::TrajectoryBasedOptimisationAlgorithm(
      const std::shared_ptr<OptimisationProblem<T, U>> optimisationProblem) noexcept
    : OptimisationAlgorithm<T, U>(optimisationProblem) {
    setInitialParameter(arma::randu<arma::Col<T>>(this->numberOfDimensions_) % (this->getUpperBounds() - this->getLowerBounds()) + this->getLowerBounds());
  }

  template <typename T, typename U>
  template <typename V, typename std::enable_if<std::is_integral<V>::value, bool>::type>
  TrajectoryBasedOptimisationAlgorithm<T, U>::TrajectoryBasedOptimisationAlgorithm(
      const std::shared_ptr<OptimisationProblem<T, U>> optimisationProblem) noexcept
    : OptimisationAlgorithm<T, U>(optimisationProblem) {
    arma::Col<T> initialParameter(this->numberOfDimensions_);
    for (std::size_t n = 0; n < initialParameter.n_elem; ++n) {
      initialParameter(n) = std::uniform_int_distribution<T>(this->getLowerBounds()(n), this->getUpperBounds()(n))(Rng::getGenerator());
    }    
    
    setInitialParameter(initialParameter);
  }

  template <typename T, typename U>
  void TrajectoryBasedOptimisationAlgorithm<T, U>::setInitialParameter(
      const arma::Col<T> initialParameter) {
    verify(initialParameter.n_elem == this->numberOfDimensions_, "The number of dimensions of the initial parameter must match the number of dimensions of the optimisation problem.");

    initialParameter_ = initialParameter;
  }
}
