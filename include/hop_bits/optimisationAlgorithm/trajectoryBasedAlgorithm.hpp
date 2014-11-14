#pragma once

// HOP
#include <hop_bits/optimisationAlgorithm.hpp>

namespace hop {
  class TrajectoryBasedAlgorithm : public OptimisationAlgorithm {
    public:
      explicit TrajectoryBasedAlgorithm(
          const std::shared_ptr<OptimisationProblem> optimisationProblem);

      void setInitialParameter(
          const arma::Col<double>& initialParameter);

    protected:
      arma::Col<double> initialParameter_;
  };
}
