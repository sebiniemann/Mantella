#pragma once

#include <optimisationProblem/benchmark/benchmarkProblem.hpp>

namespace hop {
  class WeierstrassFunction : public BenchmarkProblem {
    public:
      WeierstrassFunction(const unsigned int& numberOfDimensions);

    protected:
      double _f0;
      const Col<double> _delta;

      double getObjectiveValueImplementation(const Col<double>& parameter) const;
  };
}
