#pragma once

// Mantella
#include <mantella_bits/propertiesAnalysis/activePropertiesAnalysis/separabilityAnalysis/multiplicativeSeparabilityAnalysis.hpp>
#include <mantella_bits/distanceFunction/euclideanDistance.hpp>

namespace mant {
  class DirectMultiplicativeSeparabilityAnalysis : public MultiplicativeSeparabilityAnalysis<double, EuclideanDistance> {
    public:
      using MultiplicativeSeparabilityAnalysis<double, EuclideanDistance>::MultiplicativeSeparabilityAnalysis;

    protected:
      void analyseImplementation(
          const std::shared_ptr<OptimisationProblem<double>> optimisationProblem) noexcept override;
  };
}
