#pragma once

// HOP
#include <hop_bits/propertiesAnalysis/activePropertiesAnalysis/separabilityAnalysis/additiveSeparabilityAnalysis.hpp>

namespace hop {
  class DirectAdditiveSeparabilityAnalysis : public AdditiveSeparabilityAnalysis<double> {
    public:
      using AdditiveSeparabilityAnalysis<double>::AdditiveSeparabilityAnalysis;

    protected:
      void analyseImplementation(
          const std::shared_ptr<OptimisationProblem<double>> optimisationProblem) noexcept override;
  };
}
