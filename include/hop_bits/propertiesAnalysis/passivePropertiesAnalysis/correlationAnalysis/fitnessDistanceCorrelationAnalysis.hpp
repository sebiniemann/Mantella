#pragma once

// HOP
#include <hop_bits/propertiesAnalysis/passivePropertiesAnalysis/correlationAnalysis.hpp>

namespace hop {
  class FitnessDistanceCorrelationAnalysis : public CorrelationAnalysis {
    public:
      using CorrelationAnalysis::CorrelationAnalysis;

    protected:
      void analyseImplementation(
          const std::shared_ptr<OptimisationProblem> optimisationProblem) noexcept override;
      void analyseImplementation(
          const std::pair<arma::Col<double>, double>& parameterToObjectiveValueMapping) noexcept override;
  };
}
