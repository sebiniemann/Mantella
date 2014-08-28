#pragma once

#include <optimisationProblem/benchmark/benchmarkProblem.hpp>

namespace hop {
  class RastriginFunctionRotated : public BenchmarkProblem {
    public:
      RastriginFunctionRotated(const unsigned int& numberOfDimensions);

    protected:
      const arma::Col<double> _delta;

      double getObjectiveValueImplementation(const arma::Col<double>& parameter) const override;

      friend class cereal::access;
      RastriginFunctionRotated() = default;

      template<class T>
      void serialize(T& archive) {
        archive(cereal::make_nvp("benchmarkProblem", cereal::base_class<BenchmarkProblem>(this)));
        archive(CEREAL_NVP(_translation));
        archive(CEREAL_NVP(_rotationR));
        archive(CEREAL_NVP(_rotationQ));
      }
  };
}
