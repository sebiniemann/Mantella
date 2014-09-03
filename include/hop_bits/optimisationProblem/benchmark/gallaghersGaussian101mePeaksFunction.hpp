#pragma once

#include <hop_bits/optimisationProblem/benchmarkProblem.hpp>

namespace hop {
  class GallaghersGaussian101mePeaksFunction : public BenchmarkProblem {
    public:
      GallaghersGaussian101mePeaksFunction(const unsigned int& numberOfDimensions);

    protected:
      arma::Col<double> _weight;

      double getObjectiveValueImplementation(const arma::Col<double>& parameter) const override;

      friend class cereal::access;
      GallaghersGaussian101mePeaksFunction() = default;

      template<class T>
      void serialize(T& archive) {
        archive(cereal::make_nvp("benchmarkProblem", cereal::base_class<BenchmarkProblem>(this)));
        archive(CEREAL_NVP(_rotationR));
        archive(CEREAL_NVP(_deltaC101));
        archive(CEREAL_NVP(_localOptimaY101));
      }
  };
}
