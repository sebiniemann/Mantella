#pragma once

#include <cmath>

#include <hop_bits/optimisationProblem/benchmarkProblem.hpp>

namespace hop {
  class RastriginFunctionRotated : public BenchmarkProblem {
    public:
      using BenchmarkProblem::BenchmarkProblem;

      RastriginFunctionRotated(const RastriginFunctionRotated&) = delete;
      RastriginFunctionRotated& operator=(const RastriginFunctionRotated&) = delete;

    protected:
      const arma::Col<double> delta_ = getScaling(std::sqrt(10.0));

      double getObjectiveValueImplementation(const arma::Col<double>& parameter) const override;

      friend class cereal::access;
      RastriginFunctionRotated() = default;

      template<class T>
      void serialize(T& archive) {
        archive(cereal::make_nvp("benchmarkProblem", cereal::base_class<BenchmarkProblem>(this)));
        archive(CEREAL_NVP(translation_));
        archive(CEREAL_NVP(rotationR_));
        archive(CEREAL_NVP(rotationQ_));
      }
  };
}
