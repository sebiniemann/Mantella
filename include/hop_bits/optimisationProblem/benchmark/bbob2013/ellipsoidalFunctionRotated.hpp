#pragma once

#include <hop_bits/optimisationProblem/benchmark/blackBoxOptimisationBenchmark2013.hpp>

namespace hop {
  class EllipsoidalFunctionRotated : public BlackBoxOptimisationBenchmark2013 {
    public:
      using BlackBoxOptimisationBenchmark2013::BlackBoxOptimisationBenchmark2013;

      EllipsoidalFunctionRotated(const EllipsoidalFunctionRotated&) = delete;
      EllipsoidalFunctionRotated& operator=(const EllipsoidalFunctionRotated&) = delete;

      std::string to_string() const override;

    protected:
      const arma::Col<double> scaling_ = getScaling(1000000.0);

      double getObjectiveValueImplementation(const arma::Col<double>& parameter) const override;

      friend class cereal::access;
      EllipsoidalFunctionRotated() = default;

      template<class T>
      void serialize(T& archive) {
        archive(cereal::make_nvp("benchmarkProblem", cereal::base_class<BlackBoxOptimisationBenchmark2013>(this)));
        archive(cereal::make_nvp("translation", translation_));
        archive(cereal::make_nvp("rotationR", rotationR_));
      }
  };
}
