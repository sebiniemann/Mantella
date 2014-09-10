#pragma once

#include <cmath>

#include <hop_bits/optimisationProblem/benchmarkProblem.hpp>

namespace hop {
  class WeierstrassFunction : public BenchmarkProblem {
    public:
      explicit WeierstrassFunction(const unsigned int& numberOfDimensions);

      WeierstrassFunction(const WeierstrassFunction&) = delete;
      WeierstrassFunction& operator=(const WeierstrassFunction&) = delete;

    protected:
      double f0_;
      const arma::Col<double> delta_ = getScaling(std::sqrt(0.01));

      double getObjectiveValueImplementation(const arma::Col<double>& parameter) const override;

      friend class cereal::access;
      WeierstrassFunction() = default;

      template<class T>
      void serialize(T& archive) {
        archive(cereal::make_nvp("benchmarkProblem", cereal::base_class<BenchmarkProblem>(this)));
        archive(CEREAL_NVP(translation_));
        archive(CEREAL_NVP(rotationR_));
        archive(CEREAL_NVP(rotationQ_));
        archive(CEREAL_NVP(f0_));
      }
  };
}
