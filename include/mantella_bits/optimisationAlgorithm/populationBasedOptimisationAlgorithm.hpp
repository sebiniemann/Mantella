namespace mant {
  template <typename T, typename U = double>
  class PopulationBasedOptimisationAlgorithm : public OptimisationAlgorithm<T> {
    static_assert(arma::is_supported_elem_type<T>::value, "The parameter type T must be supported by Armadillo C++, i.e. an arithmetic types, except bool.");
    static_assert(std::is_arithmetic<U>::value, "U must be an arithmetic type.");
    
    public:
      template <typename V = T, typename std::enable_if<std::is_floating_point<V>::value, bool>::type = false>
      explicit PopulationBasedOptimisationAlgorithm(
          const std::shared_ptr<OptimisationProblem<T, U>> optimisationProblem,
          const std::size_t populationSize) noexcept;
          
      template <typename V = T, typename std::enable_if<std::is_integral<V>::value, bool>::type = false>
      explicit PopulationBasedOptimisationAlgorithm(
          const std::shared_ptr<OptimisationProblem<T, U>> optimisationProblem,
          const std::size_t populationSize) noexcept;

      void setInitialPopulation(
          const arma::Mat<T> initialPopulation);

    protected:
      const std::size_t populationSize_;
      arma::Mat<T> initialPopulation_;
  };

  //
  // Implementation
  //

  template <typename T, typename U>
  template <typename V, typename std::enable_if<std::is_floating_point<V>::value, bool>::type>
  PopulationBasedOptimisationAlgorithm<T, U>::PopulationBasedOptimisationAlgorithm(
      const std::shared_ptr<OptimisationProblem<T, U>> optimisationProblem,
      const std::size_t populationSize) noexcept
    : OptimisationAlgorithm<T>(optimisationProblem),
      populationSize_(populationSize) {
    arma::Mat<T> initialPopulation = arma::randu<arma::Mat<T>>(this->numberOfDimensions_, populationSize_);
    initialPopulation.each_col() %= this->getUpperBounds() - this->getLowerBounds();
    initialPopulation.each_col() += this->getLowerBounds();

    setInitialPopulation(initialPopulation);
  }

  template <typename T, typename U>
  template <typename V, typename std::enable_if<std::is_integral<V>::value, bool>::type>
  PopulationBasedOptimisationAlgorithm<T, U>::PopulationBasedOptimisationAlgorithm(
      const std::shared_ptr<OptimisationProblem<T, U>> optimisationProblem,
      const std::size_t populationSize) noexcept
    : OptimisationAlgorithm<T>(optimisationProblem),
      populationSize_(populationSize) {
    arma::Mat<T> initialPopulation(this->numberOfDimensions_, populationSize_);
    for (std::size_t n = 0; n < initialPopulation.n_rows; ++n) {
      initialPopulation.row(n) = arma::randi<arma::Row<T>>(populationSize_, distr_param(this->getLowerBounds()(n), this->getUpperBounds()(n)));
    }

    setInitialPopulation(initialPopulation);
  }
  
  template <typename T, typename U>
  void PopulationBasedOptimisationAlgorithm<T, U>::setInitialPopulation(
      const arma::Mat<T> initialPopulation) {
    verify(initialPopulation.n_rows == this->numberOfDimensions_, "The number of rows must match the number of dimensions of the optimisation problem.");
    verify(initialPopulation.n_rows == populationSize_, "The number of cols must match the population size.");

    initialPopulation_ = initialPopulation;
  }
}
