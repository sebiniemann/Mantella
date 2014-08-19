#pragma once

#include <optimisationProblem/benchmark/benchmarkProblem.hpp>

namespace hop {
  class SchwefelFunction : public BenchmarkProblem {
    public:
      SchwefelFunction(const unsigned int& numberOfDimensions);

    protected:
      Col<double> _delta;
      Col<double> _direction;

      double getObjectiveValueImplementation(const Col<double>& parameter) const;
  };
}
