#pragma once

#include <hop_bits/optimisationProblem/benchmarkProblem.hpp>

namespace hop {
  class EllipsoidalFunction : public BenchmarkProblem {
    public:
      using BenchmarkProblem::BenchmarkProblem;

      EllipsoidalFunction(const EllipsoidalFunction&) = delete;
      EllipsoidalFunction& operator=(const EllipsoidalFunction&) = delete;

      std::string to_string() const override;

    protected:
      const arma::Col<double> scaling_ = getScaling(1000000.0);

      double getObjectiveValueImplementation(const arma::Col<double>& parameter) const override;

      friend class cereal::access;
      EllipsoidalFunction() = default;

      template<class T>
      void serialize(T& archive) {
        archive(cereal::make_nvp("benchmarkProblem", cereal::base_class<BenchmarkProblem>(this)));
        archive(CEREAL_NVP(translation_));
      }
  };
}
