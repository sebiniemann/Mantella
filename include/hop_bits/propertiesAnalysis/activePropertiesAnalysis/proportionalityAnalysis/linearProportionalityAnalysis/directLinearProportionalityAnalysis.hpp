#pragma once

// HOP
#include <hop_bits/propertiesAnalysis/activePropertiesAnalysis/proportionalityAnalysis/linearProportionalityAnalysis.hpp>

namespace hop {
  class DirectLinearProportionalityAnalysis : public LinearProportionalityAnalysis {
    public:
      using LinearProportionalityAnalysis::LinearProportionalityAnalysis;

    protected:
      void analyseImplementation(
          const std::shared_ptr<OptimisationProblem> optimisationProblem) noexcept override;
  };
}
