#pragma once

#include <hop_bits/optimisationProblem/benchmarkProblem.hpp>

namespace hop {
  class DiscusFunction : public BenchmarkProblem {
    public:
      using BenchmarkProblem::BenchmarkProblem;

      DiscusFunction(const DiscusFunction&) = delete;
      DiscusFunction& operator=(const DiscusFunction&) = delete;

      std::string to_string() const override;

    protected:
      double getObjectiveValueImplementation(const arma::Col<double>& parameter) const override;

      friend class cereal::access;
      DiscusFunction() = default;

      template<class T>
      void serialize(T& archive) {
        archive(cereal::make_nvp("benchmarkProblem", cereal::base_class<BenchmarkProblem>(this)));
        archive(CEREAL_NVP(translation_));
        archive(CEREAL_NVP(rotationR_));
      }
  };
}
