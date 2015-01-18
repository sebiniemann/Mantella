#pragma once

// Mantella
#include <mantella_bits/distanceFunction.hpp>

namespace mant {
  class HammingDistance : public DistanceFunction<unsigned int> {
    protected:
      unsigned int getDistanceImplementation(
          const arma::Col<unsigned int>& firstParameter,
          const arma::Col<unsigned int>& secondParameter) const noexcept override;

      arma::Col<unsigned int> getNeighbourImplementation(
          const arma::Col<unsigned int>& parameter,
          const arma::Col<unsigned int>& minimalDistance,
          const arma::Col<unsigned int>& maximalDistance) const noexcept override;
  };
}
