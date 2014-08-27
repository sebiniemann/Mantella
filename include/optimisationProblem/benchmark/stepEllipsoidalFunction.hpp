#pragma once

#include <optimisationProblem/benchmark/benchmarkProblem.hpp>

namespace hop {
  class StepEllipsoidalFunction : public BenchmarkProblem {
    public:
      StepEllipsoidalFunction(const unsigned int& numberOfDimensions);

    protected:
      const arma::Col<double> _scaling;
      const arma::Col<double> _delta;

      double getObjectiveValueImplementation(const arma::Col<double>& parameter) const override;
  };
}
