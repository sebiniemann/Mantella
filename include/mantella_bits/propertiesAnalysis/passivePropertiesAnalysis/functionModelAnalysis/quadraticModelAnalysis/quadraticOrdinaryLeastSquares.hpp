#pragma once

// Mantella
#include <mantella_bits/propertiesAnalysis/passivePropertiesAnalysis/functionModelAnalysis/quadraticModelAnalysis.hpp>
#include <mantella_bits/distanceFunction/euclideanDistance.hpp>

namespace mant {
  class QuadraticOrdinaryLeastSquares : public QuadraticModelAnalysis<double, EuclideanDistance> {
    public:
      using QuadraticModelAnalysis<double, EuclideanDistance>::QuadraticModelAnalysis;

    protected:
      void analyseImplementation(
          const std::shared_ptr<OptimisationProblem<double>> optimisationProblem) noexcept override;
      void analyseImplementation(
          const std::pair<arma::Col<double>, double>& parameterToObjectiveValueMapping) noexcept override;
  };
}
