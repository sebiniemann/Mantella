#include "mantella_bits/optimisationAlgorithm/populationBasedOptimisationAlgorithm.hpp"

// C++ standard library
#include <stdexcept>

namespace mant {
  PopulationBasedOptimisationAlgorithm::PopulationBasedOptimisationAlgorithm()
      : OptimisationAlgorithm() {
    setPopulationSize(40);
  }

  void PopulationBasedOptimisationAlgorithm::optimise(
      OptimisationProblem& optimisationProblem,
      const arma::mat& initialParameters) {
    populationSize_ = initialParameters.n_cols;

    OptimisationAlgorithm::optimise(optimisationProblem, initialParameters);
  }

  void PopulationBasedOptimisationAlgorithm::setPopulationSize(
      const arma::uword populationSize) {
    if (populationSize == 0) {
      throw std::domain_error("PopulationBasedOptimisationAlgorithm.setPopulationSize: The population size must be greater than 0.");
    }

    populationSize_ = populationSize;
  }

  arma::uword PopulationBasedOptimisationAlgorithm::getPopulationSize() const {
    return populationSize_;
  }
}