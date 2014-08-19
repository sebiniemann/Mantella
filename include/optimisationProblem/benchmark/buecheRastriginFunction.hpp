#pragma once

#include <optimisationProblem/benchmark/benchmarkProblem.hpp>

namespace hop {
  class BuecheRastriginFunction : public BenchmarkProblem {
    public:
      BuecheRastriginFunction(const unsigned int& numberOfDimensions);

    protected:
      const Col<double> _scaling;

      double getObjectiveValueImplementation(const Col<double>& parameter) const;
  };
}
