#pragma once

#include <hop_bits/optimisationProblem/benchmarkProblem.hpp>

namespace hop {
  class LinearSlope : public BenchmarkProblem {
    public:
      using BenchmarkProblem::BenchmarkProblem;

      LinearSlope(const LinearSlope&) = delete;
      LinearSlope& operator=(const LinearSlope&) = delete;

      void setOne(const arma::Col<double>& one) override;

      std::string to_string() const override;

    protected:
      arma::Col<double> xOpt_;
      arma::Col<double> scaling_;
      double partiallyObjectiveValue_;

      double getObjectiveValueImplementation(const arma::Col<double>& parameter) const override;

      friend class cereal::access;
      LinearSlope() = default;

      template<class T>
      void serialize(T& archive) {
        archive(cereal::make_nvp("benchmarkProblem", cereal::base_class<BenchmarkProblem>(this)));
        archive(CEREAL_NVP(one_));
        archive(CEREAL_NVP(xOpt_));
        archive(CEREAL_NVP(scaling_));
        archive(CEREAL_NVP(partiallyObjectiveValue_));
      }
  };
}
