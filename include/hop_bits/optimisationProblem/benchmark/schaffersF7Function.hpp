#pragma once

#include <cmath>

#include <hop_bits/optimisationProblem/benchmarkProblem.hpp>

namespace hop {
  class SchaffersF7Function : public BenchmarkProblem {
    public:
      using BenchmarkProblem::BenchmarkProblem;

      SchaffersF7Function(const SchaffersF7Function&) = delete;
      SchaffersF7Function& operator=(const SchaffersF7Function&) = delete;

    protected:
      const arma::Col<double> delta_ = getScaling(std::sqrt(10.0));

      double getObjectiveValueImplementation(const arma::Col<double>& parameter) const override;

      friend class cereal::access;
      SchaffersF7Function() = default;

      template<class T>
      void serialize(T& archive) {
        archive(cereal::make_nvp("benchmarkProblem", cereal::base_class<BenchmarkProblem>(this)));
        archive(CEREAL_NVP(translation_));
        archive(CEREAL_NVP(rotationR_));
        archive(CEREAL_NVP(rotationQ_));
      }
  };
}
