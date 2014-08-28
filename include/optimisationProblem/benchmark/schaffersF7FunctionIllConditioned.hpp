#pragma once

#include <optimisationProblem/benchmark/benchmarkProblem.hpp>

namespace hop {
  class SchaffersF7FunctionIllConditioned : public BenchmarkProblem {
    public:
      SchaffersF7FunctionIllConditioned(const unsigned int& numberOfDimensions);

    protected:
      const arma::Col<double> _delta;

      double getObjectiveValueImplementation(const arma::Col<double>& parameter) const override;

      friend class cereal::access;
      SchaffersF7FunctionIllConditioned() = default;

      template<class T>
      void serialize(T& archive) {
        archive(cereal::make_nvp("benchmarkProblem", cereal::base_class<BenchmarkProblem>(this)));
        archive(CEREAL_NVP(_translation));
        archive(CEREAL_NVP(_rotationR));
        archive(CEREAL_NVP(_rotationQ));
      }
  };
}
