#pragma once

#include <optimisationProblem/benchmark/benchmarkProblem.hpp>

namespace hop {
  class DifferentPowersFunction : public BenchmarkProblem {
    public:
      DifferentPowersFunction(const unsigned int& numberOfDimensions);

    protected:
      double getObjectiveValueImplementation(const Col<double>& parameter) const;
  };
}

