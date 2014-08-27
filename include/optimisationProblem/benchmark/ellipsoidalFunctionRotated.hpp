#pragma once

#include <optimisationProblem/benchmark/benchmarkProblem.hpp>

namespace hop {
  class EllipsoidalFunctionRotated : public BenchmarkProblem {
    public:
      EllipsoidalFunctionRotated(const unsigned int& numberOfDimensions);

    protected:
      const arma::Col<double> _scaling;

      double getObjectiveValueImplementation(const arma::Col<double>& parameter) const override;
  };
}
