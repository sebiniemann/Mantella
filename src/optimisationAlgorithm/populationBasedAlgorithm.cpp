#include <hop_bits/optimisationAlgorithm/populationBasedAlgorithm.hpp>

namespace hop {
  PopulationBasedAlgorithm::PopulationBasedAlgorithm(
      const std::shared_ptr<OptimisationProblem> optimisationProblem,
      const unsigned int& populationSize) noexcept
    : OptimisationAlgorithm(optimisationProblem),
      populationSize_(populationSize) {
    arma::Mat<double> population = arma::randu<arma::Mat<double>>(optimisationProblem_->getNumberOfDimensions(), populationSize_);
    population.each_col() %= optimisationProblem_->getUpperBounds() - optimisationProblem_->getLowerBounds();
    population.each_col() += optimisationProblem_->getLowerBounds();

    setInitialPopulation(population);
  }

  void PopulationBasedAlgorithm::setInitialPopulation(
      const arma::Mat<double>& initialPopulation) {
    if(initialPopulation.n_rows != optimisationProblem_->getNumberOfDimensions()) {
      throw std::logic_error("The number of dimensions of the each parameter (" + std::to_string(initialPopulation.n_rows) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(optimisationProblem_->getNumberOfDimensions()) + ").");
    }

    initialPopulation_ = initialPopulation;
    populationSize_ = initialPopulation.n_cols;
  }
}
