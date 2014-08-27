#pragma once

#include <optimisationProblem/benchmark/benchmarkProblem.hpp>

namespace hop {
  class SphereFunction : public BenchmarkProblem {
    public:
      SphereFunction(const unsigned int& numberOfDimensions);

    protected:
      double getObjectiveValueImplementation(const arma::Col<double>& parameter) const override;
  };
}
