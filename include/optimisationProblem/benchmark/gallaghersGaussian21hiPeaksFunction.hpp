#pragma once

#include <optimisationProblem/benchmark/benchmarkProblem.hpp>

namespace hop {
  class GallaghersGaussian21hiPeaksFunction : public BenchmarkProblem {
    public:
      GallaghersGaussian21hiPeaksFunction(const unsigned int& numberOfDimensions);

    protected:
      arma::Col<double> _weight;

      double getObjectiveValueImplementation(const arma::Col<double>& parameter) const override;
  };
}

