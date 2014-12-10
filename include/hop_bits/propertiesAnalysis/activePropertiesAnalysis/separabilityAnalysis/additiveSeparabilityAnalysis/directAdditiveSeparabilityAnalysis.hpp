#pragma once

// HOP
#include <hop_bits/propertiesAnalysis/activePropertiesAnalysis/separabilityAnalysis/additiveSeparabilityAnalysis.hpp>

namespace hop {
  class DirectAdditiveSeparabilityAnalysis : public AdditiveSeparabilityAnalysis {
    public:
      using AdditiveSeparabilityAnalysis::AdditiveSeparabilityAnalysis;

    protected:
      void analyseImplementation(
          const std::shared_ptr<OptimisationProblem> optimisationProblem) noexcept override;
  };
}
