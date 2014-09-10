#pragma once

#include <cmath>

#include <hop_bits/optimisationProblem/benchmarkProblem.hpp>

namespace hop {
  class SchaffersF7FunctionIllConditioned : public BenchmarkProblem {
    public:
      using BenchmarkProblem::BenchmarkProblem;

      SchaffersF7FunctionIllConditioned(const SchaffersF7FunctionIllConditioned&) = delete;
      SchaffersF7FunctionIllConditioned& operator=(const SchaffersF7FunctionIllConditioned&) = delete;

    protected:
      const arma::Col<double> delta_ = getScaling(std::sqrt(1000.0));

      double getObjectiveValueImplementation(const arma::Col<double>& parameter) const override;

      friend class cereal::access;
      SchaffersF7FunctionIllConditioned() = default;

      template<class T>
      void serialize(T& archive) {
        archive(cereal::make_nvp("benchmarkProblem", cereal::base_class<BenchmarkProblem>(this)));
        archive(CEREAL_NVP(translation_));
        archive(CEREAL_NVP(rotationR_));
        archive(CEREAL_NVP(rotationQ_));
      }
  };
}
