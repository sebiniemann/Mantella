#pragma once

#include <optimisationProblem/benchmark/benchmarkProblem.hpp>

namespace hop {
  class LinearSlope : public BenchmarkProblem {
    public:
      LinearSlope(const unsigned int& numberOfDimensions);

      void setOne(const arma::Col<double> &one) override;

    protected:
      double _partiallyObjectiveValue;
      arma::Col<double> _xOpt;
      arma::Col<double> _scaling;

      double getObjectiveValueImplementation(const arma::Col<double>& parameter) const override;
  };
}
