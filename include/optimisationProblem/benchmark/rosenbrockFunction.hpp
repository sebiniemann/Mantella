#pragma once

#include <optimisationProblem/benchmark/benchmarkProblem.hpp>

namespace hop {
  class RosenbrockFunction : public BenchmarkProblem {
    public:
      RosenbrockFunction(const unsigned int& numberOfDimensions);

      void setTranslation(const arma::Col<double>& translation) override;

    protected:
      const double _max;

      double getObjectiveValueImplementation(const arma::Col<double>& parameter) const override;
  };
}
