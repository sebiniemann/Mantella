#pragma once

#include <armadillo>
using arma::Cube;

#include <optimisationProblem/benchmark/benchmarkProblem.hpp>

namespace hop {
  class LunacekBiRastriginFunction : public BenchmarkProblem {
    public:
      LunacekBiRastriginFunction(const unsigned int& numberOfDimensions);

    protected:
      double _mu1;
      const double _s;
      const Col<double> _delta;

      double getObjectiveValueImplementation(const Col<double>& parameter) const override;
  };
}
