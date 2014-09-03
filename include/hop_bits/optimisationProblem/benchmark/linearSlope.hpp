#pragma once

#include <hop_bits/optimisationProblem/benchmarkProblem.hpp>

namespace hop {
  class LinearSlope : public BenchmarkProblem {
    public:
      LinearSlope(const unsigned int& numberOfDimensions);

      void setOne(const arma::Col<double> &one) override;

    protected:
      double _partiallyObjectiveValue;
      arma::Col<double> _xOpt;
      arma::Col<double> _scaling;

      double getObjectiveValueImplementation(const arma::Col<double>& parameter) const override;

      friend class cereal::access;
      LinearSlope() = default;

      template<class T>
      void serialize(T& archive) {
        archive(cereal::make_nvp("benchmarkProblem", cereal::base_class<BenchmarkProblem>(this)));
        archive(CEREAL_NVP(_one));
      }
  };
}
