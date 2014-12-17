#pragma once

// HOP
#include <hop_bits/propertiesAnalysis/activePropertiesAnalysis/proportionalityAnalysis/linearProportionalityAnalysis.hpp>
#include <hop_bits/distanceFunction/euclideanDistance.hpp>

namespace hop {
  class DirectLinearProportionalityAnalysis : public LinearProportionalityAnalysis<double, EuclideanDistance> {
    public:
      using LinearProportionalityAnalysis<double, EuclideanDistance>::LinearProportionalityAnalysis;

    protected:
      void analyseImplementation(
          const std::shared_ptr<OptimisationProblem<double>> optimisationProblem) noexcept override;
  };
}
