#include "mantella_bits/optimisationAlgorithm/populationBasedOptimisationAlgorithm.hpp"

// Mantella
#include "mantella_bits/helper/assert.hpp"

namespace mant {
  PopulationBasedOptimisationAlgorithm::PopulationBasedOptimisationAlgorithm(
      const std::shared_ptr<OptimisationProblem> optimisationProblem,
      const arma::uword populationSize)
      : OptimisationAlgorithm(optimisationProblem),
        populationSize_(populationSize) {
    setInitialPopulation(getRandomPopulation());
  }

  void PopulationBasedOptimisationAlgorithm::setInitialPopulation(
      const arma::Mat<double>& initialPopulation) {
    verify(initialPopulation.n_rows == numberOfDimensions_, "The number of rows must match the number of dimensions of the optimisation problem.");
    verify(initialPopulation.n_cols == populationSize_, "The number of cols must match the population size.");

    initialPopulation_ = initialPopulation;
  }

  arma::Mat<double> PopulationBasedOptimisationAlgorithm::getRandomPopulation() const {
    arma::Mat<double> population = arma::randu<arma::Mat<double>>(numberOfDimensions_, populationSize_);
    population.each_col() %= getUpperBounds() - getLowerBounds();
    population.each_col() += getLowerBounds();

    return population;
  }
}
