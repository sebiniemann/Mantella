#pragma once

#include <optimisationProblem/benchmark/benchmarkProblem.hpp>

namespace hop {
  class SchwefelFunction : public BenchmarkProblem {
    public:
      SchwefelFunction(const unsigned int& numberOfDimensions);

    protected:
      arma::Col<double> _delta;

      double getObjectiveValueImplementation(const arma::Col<double>& parameter) const override;
  };
}
