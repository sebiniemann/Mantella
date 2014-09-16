#pragma once

#include <cmath>

#include <hop_bits/optimisationProblem/benchmarkProblem.hpp>

namespace hop {
  class BuecheRastriginFunction : public BenchmarkProblem {
    public:
      using BenchmarkProblem::BenchmarkProblem;

      BuecheRastriginFunction(const BuecheRastriginFunction&) = delete;
      BuecheRastriginFunction& operator=(const BuecheRastriginFunction&) = delete;

      std::string to_string() const override;

      void setTranslation(const arma::Col<double>& translation) override;

    protected:
      const arma::Col<double> scaling_ = getScaling(std::sqrt(10.0));

      double getObjectiveValueImplementation(const arma::Col<double>& parameter) const override;

      friend class cereal::access;
      BuecheRastriginFunction() = default;

      template<class T>
      void serialize(T& archive) {
        archive(cereal::make_nvp("benchmarkProblem", cereal::base_class<BenchmarkProblem>(this)));
        archive(CEREAL_NVP(translation_));
      }
  };
}
