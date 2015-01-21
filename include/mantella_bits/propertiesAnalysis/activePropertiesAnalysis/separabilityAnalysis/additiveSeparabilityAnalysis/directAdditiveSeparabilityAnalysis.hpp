#pragma once

// Mantella
#include <mantella_bits/propertiesAnalysis/activePropertiesAnalysis/separabilityAnalysis/additiveSeparabilityAnalysis.hpp>
#include <mantella_bits/distanceFunction/euclideanDistance.hpp>

namespace mant {
  class DirectAdditiveSeparabilityAnalysis : public AdditiveSeparabilityAnalysis<double, EuclideanDistance> {
    public:
      using AdditiveSeparabilityAnalysis<double, EuclideanDistance>::AdditiveSeparabilityAnalysis;

    protected:
      void analyseImplementation(
          const std::shared_ptr<OptimisationProblem<double>> optimisationProblem) noexcept override;
  };
}
