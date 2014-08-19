#pragma once

#include <optimisationProblem/benchmark/benchmarkProblem.hpp>

namespace hop {
  class CompositeGriewankRosenbrockFunctionF8F2 : public BenchmarkProblem {
    public:
      CompositeGriewankRosenbrockFunctionF8F2(const unsigned int& numberOfDimensions);

    protected:
      const double _max;
      const Mat<double> _rotationR;

      double getObjectiveValueImplementation(const Col<double>& parameter) const;
  };
}
