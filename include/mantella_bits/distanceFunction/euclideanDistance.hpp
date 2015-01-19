#pragma once

// Mantella
#include <mantella_bits/distanceFunction.hpp>

namespace mant {
  class EuclideanDistance : public DistanceFunction<double> {
    public:
      using DistanceFunction<double>::DistanceFunction;

    protected:
      double getDistanceImplementation(
          const arma::Col<double>& parameter) const noexcept override;

      arma::Col<double> getNeighbourImplementation(
          const arma::Col<double>& parameter,
          const double& minimalDistance,
          const double& maximalDistance) const noexcept override;
  };
}
