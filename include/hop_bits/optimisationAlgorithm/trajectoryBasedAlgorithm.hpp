#pragma once

// HOP
#include <hop_bits/optimisationAlgorithm.hpp>

namespace hop {
  class TrajectoryBasedAlgorithm : public OptimisationAlgorithm {
    public:
      using OptimisationAlgorithm::OptimisationAlgorithm;

      void setInitialParameter(
          const arma::Col<double>& initialParameter);

    protected:
      arma::Col<double> initialParameter_;
  };
}
