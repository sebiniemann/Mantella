#pragma once

// Mantella
#include <mantella_bits/distanceFunction.hpp>

namespace mant {
  class InfinityNorm : public DistanceFunction<double> {
    protected:
      double getDistanceImplementation(
          const arma::Col<double>& parameter) const noexcept override;

      arma::Col<double> getNeighbourImplementation(
          const arma::Col<double>& parameter,
          const double& minimalDistance,
          const double& maximalDistance) const noexcept override;
  };
}
