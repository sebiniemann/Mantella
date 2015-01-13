#pragma once

// Mantella
#include <mantella_bits/propertiesAnalysis/passivePropertiesAnalysis/continuityAnalysis/lipschitzContinuityAnalysis.hpp>
#include <mantella_bits/distanceFunction/euclideanDistance.hpp>

namespace mant {
  class DirectLipschitzContinuityAnalysis : public LipschitzContinuityAnalysis<double, EuclideanDistance> {
    public:
      using LipschitzContinuityAnalysis<double, EuclideanDistance>::LipschitzContinuityAnalysis;

    protected:
      void analyseImplementation(
          const std::shared_ptr<OptimisationProblem<double>> optimisationProblem) noexcept override;
      void analyseImplementation(
          const std::unordered_map<arma::Col<double>, double, Hash<arma::Col<double>>, IsKeyEqual<arma::Col<double>>>& parameterToObjectiveValueMappings) noexcept override;
      void analyseImplementation(
          const std::pair<arma::Col<double>, double>& parameterToObjectiveValueMapping) noexcept override;
  };
}
