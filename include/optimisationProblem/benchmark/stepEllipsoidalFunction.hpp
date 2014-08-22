#pragma once

#include <optimisationProblem/benchmark/benchmarkProblem.hpp>

namespace hop {
  class StepEllipsoidalFunction : public BenchmarkProblem {
    public:
      StepEllipsoidalFunction(const unsigned int& numberOfDimensions);

    protected:
      const Col<double> _scaling;
      const Col<double> _delta;

      double getObjectiveValueImplementation(const Col<double>& parameter) const;
  };
}
