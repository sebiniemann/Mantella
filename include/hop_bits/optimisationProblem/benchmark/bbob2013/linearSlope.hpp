#pragma once

#include <hop_bits/optimisationProblem/benchmark/blackBoxOptimisationBenchmark2013.hpp>

namespace hop {
  class LinearSlope : public BlackBoxOptimisationBenchmark2013 {
    public:
      using BlackBoxOptimisationBenchmark2013::BlackBoxOptimisationBenchmark2013;

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
        archive(cereal::make_nvp("benchmarkProblem", cereal::base_class<BlackBoxOptimisationBenchmark2013>(this)));
        archive(cereal::make_nvp("one", one_));
        archive(cereal::make_nvp("xOpt", xOpt_));
        archive(cereal::make_nvp("scaling", scaling_));
        archive(cereal::make_nvp("partiallyObjectiveValue", partiallyObjectiveValue_));
      }
  };
}
