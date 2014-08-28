#pragma once

#include <optimisationProblem/benchmark/benchmarkProblem.hpp>

namespace hop {
  class RosenbrockFunctionRotated : public BenchmarkProblem {
    public:
      RosenbrockFunctionRotated(const unsigned int& numberOfDimensions);

    protected:
      const double _max;

      double getObjectiveValueImplementation(const arma::Col<double>& parameter) const override;

      friend class cereal::access;
      RosenbrockFunctionRotated() = default;

      template<class T>
      void serialize(T& archive) {
        archive(cereal::make_nvp("benchmarkProblem", cereal::base_class<BenchmarkProblem>(this)));
        archive(CEREAL_NVP(_rotationR));
      }
  };
}
