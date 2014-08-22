#pragma once

#include <optimisationProblem/benchmark/benchmarkProblem.hpp>

namespace hop {
  class EllipsoidalFunctionRotated : public BenchmarkProblem {
    public:
      EllipsoidalFunctionRotated(const unsigned int& numberOfDimensions);

    protected:
      const Col<double> _scaling;

      double getObjectiveValueImplementation(const Col<double>& parameter) const;
  };
}
