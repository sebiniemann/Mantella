#pragma once

#include <optimisationProblem/benchmark/benchmarkProblem.hpp>

namespace hop {
  class LinearSlope : public BenchmarkProblem {
    public:
      LinearSlope(const unsigned int& numberOfDimensions);

    protected:
      double _partiallyObjectiveValue;
      Col<double> _xOpt;
      Col<double> _scaling;

      double getObjectiveValueImplementation(const Col<double>& parameter) const;
  };
}
