#pragma once

#include <optimisationProblem/benchmark/benchmarkProblem.hpp>

namespace hop {
  class RosenbrockFunction : public BenchmarkProblem {
    public:
      RosenbrockFunction(const unsigned int& numberOfDimensions);

    protected:
      const double _max;

      double getObjectiveValueImplementation(const Col<double>& parameter) const;
  };
}
