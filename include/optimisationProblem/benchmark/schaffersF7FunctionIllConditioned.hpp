#pragma once

#include <optimisationProblem/benchmark/benchmarkProblem.hpp>

namespace hop {
  class SchaffersF7FunctionIllConditioned : public BenchmarkProblem {
    public:
      SchaffersF7FunctionIllConditioned(const unsigned int& numberOfDimensions);

    protected:
      const Col<double> _delta;

      double getObjectiveValueImplementation(const Col<double>& parameter) const override;
  };
}
