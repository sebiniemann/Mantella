#pragma once

#include <hop_bits/optimisationProblem/benchmarkProblem.hpp>

namespace hop {
  class DifferentPowersFunction : public BenchmarkProblem {
    public:
      using BenchmarkProblem::BenchmarkProblem;

      DifferentPowersFunction(const DifferentPowersFunction&) = delete;
      DifferentPowersFunction& operator=(const DifferentPowersFunction&) = delete;

    protected:
      double getObjectiveValueImplementation(const arma::Col<double>& parameter) const override;

      friend class cereal::access;
      DifferentPowersFunction() = default;

      template<class T>
      void serialize(T& archive) {
        archive(cereal::make_nvp("benchmarkProblem", cereal::base_class<BenchmarkProblem>(this)));
        archive(CEREAL_NVP(translation_));
        archive(CEREAL_NVP(rotationR_));
      }
  };
}

