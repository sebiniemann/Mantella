#pragma once

#include <hop_bits/optimisationProblem/benchmarkProblem.hpp>

namespace hop {
  class GallaghersGaussian21hiPeaksFunction : public BenchmarkProblem {
    public:
      explicit GallaghersGaussian21hiPeaksFunction(const unsigned int& numberOfDimensions);

      GallaghersGaussian21hiPeaksFunction(const GallaghersGaussian21hiPeaksFunction&) = delete;
      GallaghersGaussian21hiPeaksFunction& operator=(const GallaghersGaussian21hiPeaksFunction&) = delete;

    protected:
      arma::Col<double> weight_;

      double getObjectiveValueImplementation(const arma::Col<double>& parameter) const override;

      friend class cereal::access;
      GallaghersGaussian21hiPeaksFunction() = default;

      template<class T>
      void serialize(T& archive) {
        archive(cereal::make_nvp("benchmarkProblem", cereal::base_class<BenchmarkProblem>(this)));
        archive(CEREAL_NVP(rotationR_));
        archive(CEREAL_NVP(deltaC21_));
        archive(CEREAL_NVP(localOptimaY21_));
        archive(CEREAL_NVP(weight_));
      }
  };
}

