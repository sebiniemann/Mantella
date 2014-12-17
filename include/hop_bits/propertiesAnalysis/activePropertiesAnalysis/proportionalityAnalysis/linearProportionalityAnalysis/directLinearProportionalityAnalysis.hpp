#pragma once

// HOP
#include <hop_bits/propertiesAnalysis/activePropertiesAnalysis/proportionalityAnalysis/linearProportionalityAnalysis.hpp>

namespace hop {
  class DirectLinearProportionalityAnalysis : public LinearProportionalityAnalysis<double> {
    public:
      using LinearProportionalityAnalysis<double>::LinearProportionalityAnalysis;

    protected:
      void analyseImplementation(
          const std::shared_ptr<OptimisationProblem<double>> optimisationProblem) noexcept override;
  };
}
