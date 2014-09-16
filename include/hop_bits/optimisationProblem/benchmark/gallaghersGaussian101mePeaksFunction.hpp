#pragma once

#include <hop_bits/optimisationProblem/benchmarkProblem.hpp>

namespace hop {
  class GallaghersGaussian101mePeaksFunction : public BenchmarkProblem {
    public:
      explicit GallaghersGaussian101mePeaksFunction(const unsigned int& numberOfDimensions);

      GallaghersGaussian101mePeaksFunction(const GallaghersGaussian101mePeaksFunction&) = delete;
      GallaghersGaussian101mePeaksFunction& operator=(const GallaghersGaussian101mePeaksFunction&) = delete;

      std::string to_string() const override;

    protected:
      arma::Col<double> weight_;

      double getObjectiveValueImplementation(const arma::Col<double>& parameter) const override;

      friend class cereal::access;
      GallaghersGaussian101mePeaksFunction() = default;

      template<class T>
      void serialize(T& archive) {
        archive(cereal::make_nvp("benchmarkProblem", cereal::base_class<BenchmarkProblem>(this)));
        archive(CEREAL_NVP(rotationR_));
        archive(CEREAL_NVP(deltaC101_));
        archive(CEREAL_NVP(localOptimaY101_));
        archive(CEREAL_NVP(weight_));
      }
  };
}
