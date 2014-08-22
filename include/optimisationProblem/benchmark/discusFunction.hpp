#pragma once

#include <optimisationProblem/benchmark/benchmarkProblem.hpp>

namespace hop {
  class DiscusFunction : public BenchmarkProblem {
    public:
      DiscusFunction(const unsigned int& numberOfDimensions);

    protected:
      double getObjectiveValueImplementation(const Col<double>& parameter) const;
  };
}
