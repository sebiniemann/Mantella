#pragma once

#include <optimisationProblem/benchmark/benchmarkProblem.hpp>

namespace hop {
  class SharpRidgeFunction : public BenchmarkProblem {
    public:
      SharpRidgeFunction(const unsigned int& numberOfDimensions);

    protected:
      const Col<double> _delta;

      double getObjectiveValueImplementation(const Col<double>& parameter) const;
  };
}

