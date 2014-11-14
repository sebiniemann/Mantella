#pragma once

// HOP
#include <hop_bits/propertiesAnalysis/lipschitzContinuityAnalysis.hpp>

namespace hop {
  class CompleteLipschitzAnalysis : public LipschitzContinuityAnalysis {
    protected:
      void analyseImplementation(
          const std::shared_ptr<OptimisationProblem> optimisationProblem) override;
  };
}
