#pragma once

#include <cmath>

#include <hop_bits/optimisationProblem/benchmarkProblem.hpp>

namespace hop {
  class LunacekBiRastriginFunction : public BenchmarkProblem {
    public:
      using BenchmarkProblem::BenchmarkProblem;

      LunacekBiRastriginFunction(const LunacekBiRastriginFunction&) = delete;
      LunacekBiRastriginFunction& operator=(const LunacekBiRastriginFunction&) = delete;

    protected:
      const arma::Col<double> delta_ = getScaling(std::sqrt(100.0));
      const double s_ = 1.0 - 0.5 / (std::sqrt(static_cast<double>(numberOfDimensions_) + 20.0) - 4.1);
      const double mu1_ = -std::sqrt((6.25 - 1) / s_);

      double getObjectiveValueImplementation(const arma::Col<double>& parameter) const override;

      friend class cereal::access;
      LunacekBiRastriginFunction() = default;

      template<class T>
      void serialize(T& archive) {
        archive(cereal::make_nvp("benchmarkProblem", cereal::base_class<BenchmarkProblem>(this)));
        archive(CEREAL_NVP(one_));
        archive(CEREAL_NVP(rotationR_));
        archive(CEREAL_NVP(rotationQ_));
        archive(CEREAL_NVP(s_));
        archive(CEREAL_NVP(mu1_));
      }
  };
}
