#pragma once

// HOP
#include <hop_bits/optimisationAlgorithm.hpp>

namespace hop {
  class PopulationBasedAlgorithm : public OptimisationAlgorithm {
    public:
      explicit PopulationBasedAlgorithm(
          const std::shared_ptr<OptimisationProblem> optimisationProblem,
          const unsigned int& populationSize) noexcept;

      void setInitialPopulation(
          const arma::Mat<double>& initialPopulation);

    protected:
      unsigned int populationSize_;
      arma::Mat<double> initialPopulation_;
  };
}
