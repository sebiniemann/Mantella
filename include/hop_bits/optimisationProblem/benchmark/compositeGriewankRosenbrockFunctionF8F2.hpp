#pragma once

#include <hop_bits/optimisationProblem/benchmarkProblem.hpp>

namespace hop {
  class CompositeGriewankRosenbrockFunctionF8F2 : public BenchmarkProblem {
    public:
      CompositeGriewankRosenbrockFunctionF8F2(const unsigned int& numberOfDimensions);

    protected:
      const double _max;

      double getObjectiveValueImplementation(const arma::Col<double>& parameter) const override;

      friend class cereal::access;
      CompositeGriewankRosenbrockFunctionF8F2() = default;

      template<class T>
      void serialize(T& archive) {
        archive(cereal::make_nvp("benchmarkProblem", cereal::base_class<BenchmarkProblem>(this)));
        archive(CEREAL_NVP(_rotationR));
      }
  };
}
