#pragma once

#include <armadillo>
using arma::Cube;

#include <optimisationProblem/benchmark/benchmarkProblem.hpp>

namespace hop {
  class LunacekBiRastriginFunction : public BenchmarkProblem {
    public:
      LunacekBiRastriginFunction(const unsigned int& numberOfDimensions);

    protected:
      Col<double> _xOpt;
      double _mu1;
      double _s;
      const Col<double> _delta;
      const Mat<double> _rotationR;
      const Mat<double> _rotationQ;

      double getObjectiveValueImplementation(const Col<double>& parameter) const;
  };
}
