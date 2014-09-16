#pragma once

#include <hop_bits/optimisationProblem/benchmarkProblem.hpp>

namespace hop {
  class EllipsoidalFunctionRotated : public BenchmarkProblem {
    public:
      using BenchmarkProblem::BenchmarkProblem;

      EllipsoidalFunctionRotated(const EllipsoidalFunctionRotated&) = delete;
      EllipsoidalFunctionRotated& operator=(const EllipsoidalFunctionRotated&) = delete;

      std::string to_string() const override;

    protected:
      const arma::Col<double> scaling_ = getScaling(1000000.0);

      double getObjectiveValueImplementation(const arma::Col<double>& parameter) const override;

      friend class cereal::access;
      EllipsoidalFunctionRotated() = default;

      template<class T>
      void serialize(T& archive) {
        archive(cereal::make_nvp("benchmarkProblem", cereal::base_class<BenchmarkProblem>(this)));
        archive(CEREAL_NVP(translation_));
        archive(CEREAL_NVP(rotationR_));
      }
  };
}
