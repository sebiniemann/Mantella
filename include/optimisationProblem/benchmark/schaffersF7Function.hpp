#pragma once

#include <optimisationProblem/benchmark/benchmarkProblem.hpp>

namespace hop {
  class SchaffersF7Function : public BenchmarkProblem {
    public:
      SchaffersF7Function(const unsigned int& numberOfDimensions);

    protected:
      const Col<double> _delta;

      double getObjectiveValueImplementation(const Col<double>& parameter) const;
  };
}
