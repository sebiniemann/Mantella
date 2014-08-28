#pragma once

#include <optimisationProblem/benchmark/benchmarkProblem.hpp>
#include <helper/cereal.hpp>

namespace hop {
  class SphereFunction : public BenchmarkProblem {
    public:
      SphereFunction(const unsigned int& numberOfDimensions);

    protected:
      double getObjectiveValueImplementation(const arma::Col<double>& parameter) const override;

      friend class cereal::access;
      SphereFunction() = default;

      template<class T>
      void serialize(T& archive) {
        archive(CEREAL_NVP(_numberOfDimensions));
        archive(CEREAL_NVP(_lowerBounds));
        archive(CEREAL_NVP(_upperBounds));
        archive(CEREAL_NVP(_parameterTranslation));
        archive(CEREAL_NVP(_parameterRotation));
        archive(CEREAL_NVP(_parameterScale));
        archive(CEREAL_NVP(_objectiveValueTranslation));
        archive(CEREAL_NVP(_objectiveValueScale));
        archive(CEREAL_NVP(_acceptableObjectiveValue));
        archive(CEREAL_NVP(_maximalNumberOfEvaluations));
        archive(CEREAL_NVP(_translation));
      }
  };
}
