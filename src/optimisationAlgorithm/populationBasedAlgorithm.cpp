#include <hop_bits/optimisationAlgorithm/populationBasedAlgorithm.hpp>

namespace hop {
  void PopulationBasedAlgorithm::setInitialPopulation(
      const arma::Mat<double>& initialPopulation) {
    initialPopulation_ = initialPopulation;
  }
}
