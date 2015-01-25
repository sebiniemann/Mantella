namespace mant {
  template <typename ParameterType, class DistanceFunction>
  class PopulationBasedAlgorithm : public OptimisationAlgorithm<ParameterType, DistanceFunction> {
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

  template <typename ParameterType, class DistanceFunction>
  PopulationBasedAlgorithm<ParameterType, DistanceFunction>::PopulationBasedAlgorithm(
      const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem,
      const unsigned int& populationSize) noexcept
    : OptimisationAlgorithm<ParameterType, DistanceFunction>(optimisationProblem),
      populationSize_(populationSize) {
    // TODO fix for discrete problems
    arma::Mat<ParameterType> population = arma::randu<arma::Mat<ParameterType>>(this->optimisationProblem_->getNumberOfDimensions(), populationSize_);
    population.each_col() %= this->optimisationProblem_->getUpperBounds() - this->optimisationProblem_->getLowerBounds();
    population.each_col() += this->optimisationProblem_->getLowerBounds();

    setInitialPopulation(population);
  }

  template <typename ParameterType, class DistanceFunction>
  void PopulationBasedAlgorithm<ParameterType, DistanceFunction>::setInitialPopulation(
      const arma::Mat<ParameterType>& initialPopulation) {
    if(initialPopulation.n_rows != this->optimisationProblem_->getNumberOfDimensions()) {
      throw std::logic_error("The number of dimensions of the each parameter (" + std::to_string(initialPopulation.n_rows) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(this->optimisationProblem_->getNumberOfDimensions()) + ").");
    }

    initialPopulation_ = initialPopulation;
    populationSize_ = initialPopulation_.n_cols;
  }
}
