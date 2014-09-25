#pragma once

#include <hop_bits/optimisationProblem/benchmark/blackBoxOptimisationBenchmark2013.hpp>

namespace hop {
  class GallaghersGaussian101mePeaksFunction : public BlackBoxOptimisationBenchmark2013 {
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
        archive(cereal::make_nvp("benchmarkProblem", cereal::base_class<BlackBoxOptimisationBenchmark2013>(this)));
        archive(cereal::make_nvp("rotationR", rotationR_));
        archive(cereal::make_nvp("deltaC101", deltaC101_));
        archive(cereal::make_nvp("localOptimaY101", localOptimaY101_));
        archive(cereal::make_nvp("weight", weight_));
      }
  };
}
