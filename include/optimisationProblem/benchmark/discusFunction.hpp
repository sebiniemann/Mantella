#pragma once

#include <optimisationProblem/benchmark/benchmarkProblem.hpp>

namespace hop {
  class DiscusFunction : public BenchmarkProblem {
    public:
      DiscusFunction(const unsigned int& numberOfDimensions);

    protected:
      const Mat<double> _rotationR;

      double getObjectiveValueImplementation(const Col<double>& parameter) const;
  };
}
