#pragma once

#include <optimisationProblem/benchmark/benchmarkProblem.hpp>

namespace hop {
  class RastriginFunction : public BenchmarkProblem {
    public:
      RastriginFunction(const unsigned int& numberOfDimensions);

    protected:
      const Col<double> _delta;

      double getObjectiveValueImplementation(const Col<double>& parameter) const;
  };
}
