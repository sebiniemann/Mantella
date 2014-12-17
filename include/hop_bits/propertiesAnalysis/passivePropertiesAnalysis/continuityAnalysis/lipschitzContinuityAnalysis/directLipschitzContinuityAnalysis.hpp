#pragma once

// HOP
#include <hop_bits/propertiesAnalysis/passivePropertiesAnalysis/continuityAnalysis/lipschitzContinuityAnalysis.hpp>
#include <hop_bits/distanceFunction/euclideanDistance.hpp>

namespace hop {
  class DirectLipschitzContinuityAnalysis : public LipschitzContinuityAnalysis<double, EuclideanDistance> {
    public:
      using LipschitzContinuityAnalysis<double, EuclideanDistance>::LipschitzContinuityAnalysis;

    protected:
      void analyseImplementation(
          const std::shared_ptr<OptimisationProblem<double>> optimisationProblem) noexcept override;
      void analyseImplementation(
          const std::pair<arma::Col<double>, double>& parameterToObjectiveValueMapping) noexcept override;
  };
}
