#pragma once

#include <cmath>

#include <hop_bits/optimisationProblem/benchmark/blackBoxOptimisationBenchmark2013.hpp>

namespace hop {
  class KatsuuraFunction : public BlackBoxOptimisationBenchmark2013 {
    public:
      using BlackBoxOptimisationBenchmark2013::BlackBoxOptimisationBenchmark2013;

      KatsuuraFunction(const KatsuuraFunction&) = delete;
      KatsuuraFunction& operator=(const KatsuuraFunction&) = delete;

      std::string to_string() const override;

    protected:
      const arma::Col<double> delta_ = getScaling(std::sqrt(100.0));

      double getObjectiveValueImplementation(const arma::Col<double>& parameter) const override;

      friend class cereal::access;
      KatsuuraFunction() = default;

      template<class T>
      void serialize(T& archive) {
        archive(cereal::make_nvp("benchmarkProblem", cereal::base_class<BlackBoxOptimisationBenchmark2013>(this)));
        archive(cereal::make_nvp("translation", translation_));
        archive(cereal::make_nvp("rotationR", rotationR_));
        archive(cereal::make_nvp("rotationQ", rotationQ_));
      }
  };
}
