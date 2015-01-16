#include <mantella_bits/distanceFunction/hammingDistance.hpp>

// C++ Standard Library
#include <cmath>

// Mantella
#include <mantella_bits/helper/rng.hpp>

namespace mant {
  unsigned int HammingDistance::getDistanceImplementation(
      const arma::Col<unsigned int>& firstParameter,
      const arma::Col<unsigned int>& secondParameter) const noexcept {
    return std::abs(arma::accu(secondParameter != 0) - arma::accu(firstParameter != 0));
  }

  arma::Col<unsigned int> HammingDistance::getNeighourImplementation(
      const arma::Col<unsigned int>& parameter,
      const arma::Col<unsigned int>& minimalDistance,
      const arma::Col<unsigned int>& maximalDistance) const noexcept {
    // TODO implement
  }
}
