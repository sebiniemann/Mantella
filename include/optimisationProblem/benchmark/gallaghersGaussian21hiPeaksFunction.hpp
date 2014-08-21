#pragma once

#include <armadillo>
using arma::Cube;

#include <optimisationProblem/benchmark/benchmarkProblem.hpp>

namespace hop {
  class GallaghersGaussian21hiPeaksFunction : public BenchmarkProblem {
    public:
      GallaghersGaussian21hiPeaksFunction(const unsigned int& numberOfDimensions);

    protected:
      Mat<double> _delta21;
      const Mat<double> _rotationR;
      Mat<double> _localOptima21;
      Col<double> _weight;

      double getObjectiveValueImplementation(const Col<double>& parameter) const;
  };
}

