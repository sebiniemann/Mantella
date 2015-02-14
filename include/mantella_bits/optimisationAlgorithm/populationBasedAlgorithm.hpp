namespace mant {
  template <typename ParameterType>
  class PopulationBasedAlgorithm : public OptimisationAlgorithm<ParameterType> {
    public:
      explicit PopulationBasedAlgorithm(
          const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem,
          const unsigned int& populationSize) noexcept;

      void setInitialPopulation(
          const arma::Mat<ParameterType>& initialPopulation);

    protected:
      unsigned int populationSize_;
      arma::Mat<ParameterType> initialPopulation_;
  };

  //
  // Implementation
  //

  template <typename ParameterType>
  PopulationBasedAlgorithm<ParameterType>::PopulationBasedAlgorithm(
      const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem,
      const unsigned int& populationSize) noexcept
    : OptimisationAlgorithm<ParameterType>(optimisationProblem),
      populationSize_(populationSize) {
    // TODO fix for discrete problems
    arma::Mat<ParameterType> population = arma::randu<arma::Mat<ParameterType>>(this->optimisationProblem_->numberOfDimensions_, populationSize_);
    population.each_col() %= this->optimisationProblem_->getUpperBounds() - this->optimisationProblem_->getLowerBounds();
    population.each_col() += this->optimisationProblem_->getLowerBounds();

    setInitialPopulation(population);
  }

  template <typename ParameterType>
  void PopulationBasedAlgorithm<ParameterType>::setInitialPopulation(
      const arma::Mat<ParameterType>& initialPopulation) {
    if(initialPopulation.n_rows != this->optimisationProblem_->numberOfDimensions_) {
      throw std::logic_error("The number of dimensions of the each parameter (" + std::to_string(initialPopulation.n_rows) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(this->optimisationProblem_->numberOfDimensions_) + ").");
    }

    initialPopulation_ = initialPopulation;
    populationSize_ = initialPopulation_.n_cols;
  }
}
