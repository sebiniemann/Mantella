#pragma once

// HOP
#include <hop_bits/propertiesAnalysis/passivePropertiesAnalysis/correlationAnalysis.hpp>
#include <hop_bits/distanceFunction/euclideanDistance.hpp>

namespace hop {
  class FitnessDistanceCorrelationAnalysis : public CorrelationAnalysis<double, EuclideanDistance> {
    public:
      using CorrelationAnalysis::CorrelationAnalysis;

    protected:
      void analyseImplementation(
          const std::shared_ptr<OptimisationProblem<double>> optimisationProblem) noexcept override;
      void analyseImplementation(
          const std::pair<arma::Col<double>, double>& parameterToObjectiveValueMapping) noexcept override;
  };
}
