#pragma once

#include <optimisationProblem/benchmark/benchmarkProblem.hpp>

namespace hop {
  class WeierstrassFunction : public BenchmarkProblem {
    public:
      WeierstrassFunction(const unsigned int& numberOfDimensions);

    protected:
      double _f0;
      const arma::Col<double> _delta;

      double getObjectiveValueImplementation(const arma::Col<double>& parameter) const override;
  };
}
