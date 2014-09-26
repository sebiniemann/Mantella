#pragma once

#include <cmath>

#include <hop_bits/optimisationProblem/benchmark/blackBoxOptimisationBenchmark2013.hpp>

namespace hop {
  class WeierstrassFunction : public BlackBoxOptimisationBenchmark2013 {
    public:
      explicit WeierstrassFunction(const unsigned int& numberOfDimensions);

      WeierstrassFunction(const WeierstrassFunction&) = delete;
      WeierstrassFunction& operator=(const WeierstrassFunction&) = delete;

      std::string to_string() const override;

    protected:
      double f0_;
      const arma::Col<double> delta_ = getScaling(std::sqrt(0.01));

      double getObjectiveValueImplementation(const arma::Col<double>& parameter) const override;

      friend class cereal::access;
      WeierstrassFunction() = default;

      template<class T>
      void serialize(T& archive) {
        archive(cereal::make_nvp("benchmarkProblem", cereal::base_class<BlackBoxOptimisationBenchmark2013>(this)));
        archive(cereal::make_nvp("translation", translation_));
        archive(cereal::make_nvp("rotationR", rotationR_));
        archive(cereal::make_nvp("rotationQ", rotationQ_));
        archive(cereal::make_nvp("f0", f0_));
      }
  };
}
