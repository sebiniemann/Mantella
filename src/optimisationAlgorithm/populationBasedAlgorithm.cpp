#include <hop_bits/optimisationAlgorithm/populationBasedAlgorithm.hpp>

namespace hop {
  PopulationBasedAlgorithm::PopulationBasedAlgorithm(
      const std::shared_ptr<OptimisationProblem> optimisationProblem,
      const unsigned int& populationSize)
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
      // TODO Add exception
    }

    initialPopulation_ = initialPopulation;
    populationSize_ = initialPopulation.n_cols;
  }
}
