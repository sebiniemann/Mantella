#pragma once

// Mantella
#include <mantella_bits/distanceFunction.hpp>

namespace mant {
  class HammingDistance : public DistanceFunction<unsigned int> {
    protected:
      unsigned int getDistanceImplementation(
          const arma::Col<unsigned int>& parameter) const noexcept override;

      arma::Col<unsigned int> getNeighbourImplementation(
          const arma::Col<unsigned int>& parameter,
          const unsigned int& minimalDistance,
          const unsigned int& maximalDistance) const noexcept override;
  };
}
