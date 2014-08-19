#pragma once

#include <optimisationProblem/benchmark/benchmarkProblem.hpp>

namespace hop {
  class StepEllipsoidalFunction : public BenchmarkProblem {
    public:
      StepEllipsoidalFunction(const unsigned int& numberOfDimensions);

    protected:
      const Col<double> _scaling;
      const Col<double> _delta;
      const Mat<double> _rotationQ;
      const Mat<double> _rotationR;

      double getObjectiveValueImplementation(const Col<double>& parameter) const;
  };
}
