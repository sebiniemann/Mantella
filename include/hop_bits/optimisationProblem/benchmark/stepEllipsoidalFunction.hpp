#pragma once

#include <cmath>

#include <hop_bits/optimisationProblem/benchmarkProblem.hpp>

namespace hop {
  class StepEllipsoidalFunction : public BenchmarkProblem {
    public:
      using BenchmarkProblem::BenchmarkProblem;

      StepEllipsoidalFunction(const StepEllipsoidalFunction&) = delete;
      StepEllipsoidalFunction& operator=(const StepEllipsoidalFunction&) = delete;

    protected:
      const arma::Col<double> scaling_ = getScaling(100.0);
      const arma::Col<double> delta_ = getScaling(std::sqrt(10.0));

      double getObjectiveValueImplementation(const arma::Col<double>& parameter) const override;

      friend class cereal::access;
      StepEllipsoidalFunction() = default;

      template<class T>
      void serialize(T& archive) {
        archive(cereal::make_nvp("benchmarkProblem", cereal::base_class<BenchmarkProblem>(this)));
        archive(CEREAL_NVP(translation_));
        archive(CEREAL_NVP(rotationR_));
        archive(CEREAL_NVP(rotationQ_));
      }
  };
}
