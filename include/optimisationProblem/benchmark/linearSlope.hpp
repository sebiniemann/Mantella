#pragma once

#include <optimisationProblem/benchmark/benchmarkProblem.hpp>

namespace hop {
  class LinearSlope : public BenchmarkProblem {
    public:
      LinearSlope(const unsigned int& numberOfDimensions);

      void setOne(const Col<double> &one) override;

    protected:
      double _partiallyObjectiveValue;
      Col<double> _xOpt;
      Col<double> _scaling;

      double getObjectiveValueImplementation(const Col<double>& parameter) const override;
  };
}
