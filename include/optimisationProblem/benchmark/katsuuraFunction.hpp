#pragma once

#include <armadillo>
using arma::Cube;

#include <optimisationProblem/benchmark/benchmarkProblem.hpp>

namespace hop {
  class KatsuuraFunction : public BenchmarkProblem {
    public:
      KatsuuraFunction(const unsigned int& numberOfDimensions);

    protected:
      const Col<double> _delta;
      const Mat<double> _rotationR;
      const Mat<double> _rotationQ;

      double getObjectiveValueImplementation(const Col<double>& parameter) const;
  };
}
