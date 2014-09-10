#pragma once

#include <hop_bits/optimisationProblem/benchmarkProblem.hpp>

namespace hop {
  class SphereFunction : public BenchmarkProblem {
    public:
      using BenchmarkProblem::BenchmarkProblem;

      SphereFunction(const SphereFunction&) = delete;
      SphereFunction& operator=(const SphereFunction&) = delete;

    protected:
      double getObjectiveValueImplementation(const arma::Col<double>& parameter) const override;

      friend class cereal::access;
      SphereFunction() = default;

      template<class T>
      void serialize(T& archive) {
        archive(cereal::make_nvp("benchmarkProblem", cereal::base_class<BenchmarkProblem>(this)));
        archive(CEREAL_NVP(translation_));
      }
  };
}
