#pragma once

// HOP
#include <hop_bits/propertiesAnalysis/functionModelAnalysis/linearModelAnalysis.hpp>

namespace hop {
  class LinearOrdinaryLeastSquares : public LinearModelAnalysis {
    protected:
      void analyseImplementation(
          const std::shared_ptr<OptimisationProblem> optimisationProblem) noexcept override;
  };
}
