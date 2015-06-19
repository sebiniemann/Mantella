namespace mant {
  template <typename T = double>
  class PopulationBasedOptimisationAlgorithm : public OptimisationAlgorithm<T> {
    static_assert(std::is_floating_point<T>::value, "The parameter type T must be a floating point type.");
    
    public:
      explicit PopulationBasedOptimisationAlgorithm(
          const std::shared_ptr<OptimisationProblem<T>> optimisationProblem,
          const std::size_t populationSize) noexcept;

      void setInitialPopulation(
          const arma::Mat<T> initialPopulation);

      arma::Mat<T> getRandomPopulation() const noexcept;

    protected:
      const std::size_t populationSize_;
      arma::Mat<T> initialPopulation_;
  };

  //
  // Implementation
  //

  template <typename T>
  PopulationBasedOptimisationAlgorithm<T>::PopulationBasedOptimisationAlgorithm(
      const std::shared_ptr<OptimisationProblem<T>> optimisationProblem,
      const std::size_t populationSize) noexcept
    : OptimisationAlgorithm<T>(optimisationProblem),
      populationSize_(populationSize) {
    setInitialPopulation(getRandomPopulation());
  }
  
  template <typename T>
  void PopulationBasedOptimisationAlgorithm<T>::setInitialPopulation(
      const arma::Mat<T> initialPopulation) {
    verify(initialPopulation.n_rows == this->numberOfDimensions_, "The number of rows must match the number of dimensions of the optimisation problem.");
    verify(initialPopulation.n_rows == populationSize_, "The number of cols must match the population size.");

    initialPopulation_ = initialPopulation;
  }
  
  template <typename T>
  arma::Mat<T> PopulationBasedOptimisationAlgorithm<T>::getRandomPopulation() const noexcept {
    arma::Mat<T> population = arma::randu<arma::Mat<T>>(this->numberOfDimensions_, populationSize_);
    population.each_col() %= this->getUpperBounds() - this->getLowerBounds();
    population.each_col() += this->getLowerBounds();
    
    return population;
  }
}
