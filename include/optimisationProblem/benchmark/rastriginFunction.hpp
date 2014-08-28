#pragma once

#include <optimisationProblem/benchmark/benchmarkProblem.hpp>

namespace hop {
  class RastriginFunction : public BenchmarkProblem {
    public:
      RastriginFunction(const unsigned int& numberOfDimensions);

    protected:
      const arma::Col<double> _delta;

      double getObjectiveValueImplementation(const arma::Col<double>& parameter) const override;

      friend class cereal::access;
      RastriginFunction() = default;

      template<class T>
      void serialize(T& archive) {
        archive(cereal::make_nvp("benchmarkProblem", cereal::base_class<BenchmarkProblem>(this)));
        archive(CEREAL_NVP(_translation));
      }
  };
}
