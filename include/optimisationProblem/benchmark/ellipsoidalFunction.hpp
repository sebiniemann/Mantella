#pragma once

#include <optimisationProblem/benchmark/benchmarkProblem.hpp>

namespace hop {
  class EllipsoidalFunction : public BenchmarkProblem {
    public:
      EllipsoidalFunction(const unsigned int& numberOfDimensions);

    protected:
      const Col<double> _scaling;

      double getObjectiveValueImplementation(const Col<double>& parameter) const override;
  };
}
