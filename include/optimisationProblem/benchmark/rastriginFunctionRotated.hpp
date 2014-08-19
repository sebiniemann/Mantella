#pragma once

#include <optimisationProblem/benchmark/benchmarkProblem.hpp>

namespace hop {
  class RastriginFunctionRotated : public BenchmarkProblem {
    public:
      RastriginFunctionRotated(const unsigned int& numberOfDimensions);

    protected:
      const Col<double> _delta;
      const Mat<double> _rotationR;
      const Mat<double> _rotationQ;

      double getObjectiveValueImplementation(const Col<double>& parameter) const;
  };
}
