#pragma once

#include <cmath>

#include <hop_bits/optimisationProblem/benchmarkProblem.hpp>

namespace hop {
  class SchwefelFunction : public BenchmarkProblem {
    public:
      using BenchmarkProblem::BenchmarkProblem;

      SchwefelFunction(const SchwefelFunction&) = delete;
      SchwefelFunction& operator=(const SchwefelFunction&) = delete;

      std::string to_string() const override;

    protected:
      arma::Col<double> delta_ = getScaling(std::sqrt(10));

      double getObjectiveValueImplementation(const arma::Col<double>& parameter) const override;

      friend class cereal::access;
      SchwefelFunction() = default;

      template<class T>
      void serialize(T& archive) {
        archive(cereal::make_nvp("benchmarkProblem", cereal::base_class<BenchmarkProblem>(this)));
        archive(CEREAL_NVP(one_));
      }
  };
}
