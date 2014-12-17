#pragma once

// HOP
#include <hop_bits/propertiesAnalysis/passivePropertiesAnalysis/functionModelAnalysis/linearModelAnalysis.hpp>

namespace hop {
  class LinearOrdinaryLeastSquares : public LinearModelAnalysis<double> {
    public:
      using LinearModelAnalysis<double>::LinearModelAnalysis;

    protected:
      void analyseImplementation(
          const std::shared_ptr<OptimisationProblem<double>> optimisationProblem) noexcept override;
      void analyseImplementation(
          const std::pair<arma::Col<double>, double>& parameterToObjectiveValueMapping) noexcept override;
  };
}
