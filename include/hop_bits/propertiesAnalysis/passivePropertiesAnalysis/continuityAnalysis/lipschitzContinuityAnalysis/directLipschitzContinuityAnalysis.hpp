#pragma once

// HOP
#include <hop_bits/propertiesAnalysis/passivePropertiesAnalysis/continuityAnalysis/lipschitzContinuityAnalysis.hpp>

namespace hop {
  class DirectLipschitzContinuityAnalysis : public LipschitzContinuityAnalysis<double> {
    public:
      using LipschitzContinuityAnalysis<double>::LipschitzContinuityAnalysis;

    protected:
      void analyseImplementation(
          const std::shared_ptr<OptimisationProblem<double>> optimisationProblem) noexcept override;
      void analyseImplementation(
          const std::pair<arma::Col<double>, double>& parameterToObjectiveValueMapping) noexcept override;
  };
}
