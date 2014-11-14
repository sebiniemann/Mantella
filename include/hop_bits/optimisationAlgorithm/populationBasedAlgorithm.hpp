#pragma once

// HOP
#include <hop_bits/optimisationAlgorithm.hpp>

namespace hop {
  class PopulationBasedAlgorithm : public OptimisationAlgorithm {
    public:
      using OptimisationAlgorithm::OptimisationAlgorithm;

      void setInitialPopulation(
          const arma::Mat<double>& initialPopulation);

    protected:
      arma::Mat<double> initialPopulation_;
  };
}
