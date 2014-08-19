#pragma once

#include <armadillo>
using arma::Cube;

#include <optimisationProblem/benchmark/benchmarkProblem.hpp>

namespace hop {
  class GallaghersGaussian21hiPeaksFunction : public BenchmarkProblem {
    public:
      GallaghersGaussian21hiPeaksFunction(const unsigned int& numberOfDimensions);

    protected:
      Mat<double> _delta;
      const Mat<double> _rotationR;
      Mat<double> _localOptima;
      Col<double> _weight;

      double getObjectiveValueImplementation(const Col<double>& parameter) const;
  };
}

