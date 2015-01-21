#pragma once

// Mantella
#include <mantella_bits/propertiesAnalysis/activePropertiesAnalysis/proportionalityAnalysis/linearProportionalityAnalysis.hpp>
#include <mantella_bits/distanceFunction/euclideanDistance.hpp>

namespace mant {
  class DirectLinearProportionalityAnalysis : public LinearProportionalityAnalysis<double, EuclideanDistance> {
    public:
      using LinearProportionalityAnalysis<double, EuclideanDistance>::LinearProportionalityAnalysis;

    protected:
      void analyseImplementation(
          const std::shared_ptr<OptimisationProblem<double>> optimisationProblem) noexcept override;
  };
}
