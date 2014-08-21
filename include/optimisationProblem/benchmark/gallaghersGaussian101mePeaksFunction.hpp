#pragma once

#include <armadillo>
using arma::Cube;

#include <optimisationProblem/benchmark/benchmarkProblem.hpp>

namespace hop {
  class GallaghersGaussian101mePeaksFunction : public BenchmarkProblem {
    public:
      GallaghersGaussian101mePeaksFunction(const unsigned int& numberOfDimensions);

    protected:
      Mat<double> _delta101;
      const Mat<double> _rotationR;
      Mat<double> _localOptima101;
      Col<double> _weight;

      double getObjectiveValueImplementation(const Col<double>& parameter) const;
  };
}
