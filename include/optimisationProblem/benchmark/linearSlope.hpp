#pragma once

#include <optimisationProblem/benchmark/benchmarkProblem.hpp>

namespace hop {
  class LinearSlope : public BenchmarkProblem {
    public:
      LinearSlope(const unsigned int& numberOfDimensions);

    protected:
      double _partiallyObjectiveValue;
      Col<double> _scaling;
      Col<double> _direction;

      double getObjectiveValueImplementation(const Col<double>& parameter) const;
  };
}
