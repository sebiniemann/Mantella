namespace mant {
  template <typename T>
  class PopulationBasedOptimisationAlgorithm : public OptimisationAlgorithm<T> {
    public:
      explicit PopulationBasedOptimisationAlgorithm(
          const std::shared_ptr<OptimisationProblem<T>> optimisationProblem,
          const unsigned int populationSize) noexcept;

      void setInitialPopulation(
          const arma::Mat<T> initialPopulation);

    protected:
      unsigned int populationSize_;
      arma::Mat<T> initialPopulation_;
  };

  //
  // Implementation
  //

  template <typename T>
  PopulationBasedOptimisationAlgorithm<T>::PopulationBasedOptimisationAlgorithm(
      const std::shared_ptr<OptimisationProblem<T>> optimisationProblem,
      const unsigned int populationSize) noexcept
    : OptimisationAlgorithm<T>(optimisationProblem),
      populationSize_(populationSize) {
    // TODO fix for discrete problems
    arma::Mat<T> population = arma::randu<arma::Mat<T>>(this->numberOfDimensions_, populationSize_);
    population.each_col() %= this->getUpperBounds() - this->getLowerBounds();
    population.each_col() += this->getLowerBounds();

    setInitialPopulation(population);
  }

  template <typename T>
  void PopulationBasedOptimisationAlgorithm<T>::setInitialPopulation(
      const arma::Mat<T> initialPopulation) {
    if(initialPopulation.n_rows != this->numberOfDimensions_) {
      throw std::logic_error("The number of dimensions of the each parameter (" + std::to_string(initialPopulation.n_rows) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(this->numberOfDimensions_) + ").");
    }

    initialPopulation_ = initialPopulation;
    populationSize_ = initialPopulation_.n_cols;
  }
}
