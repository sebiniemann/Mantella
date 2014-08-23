#pragma once

#include <optimisationProblem/benchmark/benchmarkProblem.hpp>

namespace hop {
  class BentCigarFunction : public BenchmarkProblem {
    public:
      BentCigarFunction(const unsigned int& numberOfDimensions);

    protected:
      double getObjectiveValueImplementation(const Col<double>& parameter) const override;
  };
}
