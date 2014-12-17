#pragma once

// HOP
#include <hop_bits/propertiesAnalysis/passivePropertiesAnalysis/functionModelAnalysis/linearModelAnalysis.hpp>
#include <hop_bits/distanceFunction/euclideanDistance.hpp>

namespace hop {
  class LinearOrdinaryLeastSquares : public LinearModelAnalysis<double, EuclideanDistance> {
    public:
      using LinearModelAnalysis<double, EuclideanDistance>::LinearModelAnalysis;

    protected:
      void analyseImplementation(
          const std::shared_ptr<OptimisationProblem<double>> optimisationProblem) noexcept override;
      void analyseImplementation(
          const std::pair<arma::Col<double>, double>& parameterToObjectiveValueMapping) noexcept override;
  };
}
