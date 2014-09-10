#pragma once

#include <hop_bits/optimisationProblem/benchmarkProblem.hpp>

namespace hop {
  class BentCigarFunction : public BenchmarkProblem {
    public:
      using BenchmarkProblem::BenchmarkProblem;

      BentCigarFunction(const BentCigarFunction&) = delete;
      BentCigarFunction& operator=(const BentCigarFunction&) = delete;

    protected:
      double getObjectiveValueImplementation(const arma::Col<double>& parameter) const override;

      friend class cereal::access;
      BentCigarFunction() = default;

      template<class T>
      void serialize(T& archive) {
        archive(cereal::make_nvp("benchmarkProblem", cereal::base_class<BenchmarkProblem>(this)));
        archive(CEREAL_NVP(translation_));
        archive(CEREAL_NVP(rotationR_));
      }
  };
}
