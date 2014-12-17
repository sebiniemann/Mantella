#pragma once

// HOP
#include <hop_bits/propertiesAnalysis/activePropertiesAnalysis/separabilityAnalysis/additiveSeparabilityAnalysis.hpp>
#include <hop_bits/distanceFunction/euclideanDistance.hpp>

namespace hop {
  class DirectAdditiveSeparabilityAnalysis : public AdditiveSeparabilityAnalysis<double, EuclideanDistance> {
    public:
      using AdditiveSeparabilityAnalysis<double, EuclideanDistance>::AdditiveSeparabilityAnalysis;

    protected:
      void analyseImplementation(
          const std::shared_ptr<OptimisationProblem<double>> optimisationProblem) noexcept override;
  };
}
