#pragma once

#include <optimisationProblem/benchmark/benchmarkProblem.hpp>

namespace hop {
  class RosenbrockFunctionRotated : public BenchmarkProblem {
    public:
      RosenbrockFunctionRotated(const unsigned int& numberOfDimensions);

    protected:
      const double _max;
      const Mat<double> _rotationR;

      double getObjectiveValueImplementation(const Col<double>& parameter) const;
  };
}
