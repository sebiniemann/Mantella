#pragma once

#include <optimisationProblem/benchmark/benchmarkProblem.hpp>

namespace hop {
  class BuecheRastriginFunction : public BenchmarkProblem {
    public:
      BuecheRastriginFunction(const unsigned int& numberOfDimensions);

      void setTranslation(const arma::Col<double> &translation) override;

    protected:
      const arma::Col<double> _scaling;

      double getObjectiveValueImplementation(const arma::Col<double>& parameter) const override;
  };
}
