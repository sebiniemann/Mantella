#pragma once

#include <algorithm>
#include <cmath>

#include <hop_bits/optimisationProblem/benchmarkProblem.hpp>

namespace hop {
  class CompositeGriewankRosenbrockFunctionF8F2 : public BenchmarkProblem {
    public:
      using BenchmarkProblem::BenchmarkProblem;

      CompositeGriewankRosenbrockFunctionF8F2(const CompositeGriewankRosenbrockFunctionF8F2&) = delete;
      CompositeGriewankRosenbrockFunctionF8F2& operator=(const CompositeGriewankRosenbrockFunctionF8F2&) = delete;

    protected:
      const double max_ = std::max(1.0, std::sqrt(static_cast<double>(numberOfDimensions_)) / 8.0);

      double getObjectiveValueImplementation(const arma::Col<double>& parameter) const override;

      friend class cereal::access;
      CompositeGriewankRosenbrockFunctionF8F2() = default;

      template<class T>
      void serialize(T& archive) {
        archive(cereal::make_nvp("benchmarkProblem", cereal::base_class<BenchmarkProblem>(this)));
        archive(CEREAL_NVP(rotationR_));
      }
  };
}
