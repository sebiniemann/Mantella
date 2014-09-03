#pragma once

#include <hop_bits/optimisationProblem/benchmarkProblem.hpp>

namespace hop {
  class GallaghersGaussian21hiPeaksFunction : public BenchmarkProblem {
    public:
      GallaghersGaussian21hiPeaksFunction(const unsigned int& numberOfDimensions);

    protected:
      arma::Col<double> _weight;

      double getObjectiveValueImplementation(const arma::Col<double>& parameter) const override;

      friend class cereal::access;
      GallaghersGaussian21hiPeaksFunction() = default;

      template<class T>
      void serialize(T& archive) {
        archive(cereal::make_nvp("benchmarkProblem", cereal::base_class<BenchmarkProblem>(this)));
        archive(CEREAL_NVP(_rotationR));
        archive(CEREAL_NVP(_deltaC21));
        archive(CEREAL_NVP(_localOptimaY21));
      }
  };
}

