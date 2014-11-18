#pragma once

// HOP
#include <hop_bits/propertiesAnalysis/continuityAnalysis/lipschitzContinuityAnalysis.hpp>

namespace hop {
  class DirectLipschitzContinuityAnalysis : public LipschitzContinuityAnalysis {
    protected:
      void analyseImplementation(
          const std::shared_ptr<OptimisationProblem> optimisationProblem) override;
  };
}
