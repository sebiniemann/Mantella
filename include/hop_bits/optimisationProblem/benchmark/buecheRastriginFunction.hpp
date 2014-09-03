#pragma once

#include <hop_bits/optimisationProblem/benchmarkProblem.hpp>

namespace hop {
  class BuecheRastriginFunction : public BenchmarkProblem {
    public:
      BuecheRastriginFunction(const unsigned int& numberOfDimensions);

      void setTranslation(const arma::Col<double> &translation) override;

    protected:
      const arma::Col<double> _scaling;

      double getObjectiveValueImplementation(const arma::Col<double>& parameter) const override;

      friend class cereal::access;
      BuecheRastriginFunction() = default;

      template<class T>
      void serialize(T& archive) {
        archive(cereal::make_nvp("benchmarkProblem", cereal::base_class<BenchmarkProblem>(this)));
        archive(CEREAL_NVP(_translation));
      }
  };
}
