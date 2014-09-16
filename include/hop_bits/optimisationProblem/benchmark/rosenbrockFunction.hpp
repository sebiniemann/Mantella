#pragma once

#include <algorithm>
#include <cmath>

#include <hop_bits/optimisationProblem/benchmarkProblem.hpp>

namespace hop {
  class RosenbrockFunction : public BenchmarkProblem {
    public:
      using BenchmarkProblem::BenchmarkProblem;

      RosenbrockFunction(const RosenbrockFunction&) = delete;
      RosenbrockFunction& operator=(const RosenbrockFunction&) = delete;

      void setTranslation(const arma::Col<double>& translation) override;

      std::string to_string() const override;

    protected:
      const double max_ = std::max(1.0, std::sqrt(static_cast<double>(numberOfDimensions_)) / 8.0);

      double getObjectiveValueImplementation(const arma::Col<double>& parameter) const override;

      friend class cereal::access;
      RosenbrockFunction() = default;

      template<class T>
      void serialize(T& archive) {
        archive(cereal::make_nvp("benchmarkProblem", cereal::base_class<BenchmarkProblem>(this)));
        archive(CEREAL_NVP(translation_));
      }
  };
}
