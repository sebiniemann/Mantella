#pragma once

#include <optimisationProblem/benchmark/benchmarkProblem.hpp>

namespace hop {
  class AttractiveSectorFunction : public BenchmarkProblem {
    public:
      AttractiveSectorFunction(const unsigned int& numberOfDimensions);

    protected:
      const Col<double> _delta;

      double getObjectiveValueImplementation(const Col<double>& parameter) const override;
  };
}
