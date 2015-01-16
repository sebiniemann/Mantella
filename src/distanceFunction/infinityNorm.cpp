#include <mantella_bits/distanceFunction/infinityNorm.hpp>

// Mantella
#include <mantella_bits/helper/rng.hpp>

namespace mant {
  double InfinityNorm::getDistanceImplementation(
      const arma::Col<double>& firstParameter,
      const arma::Col<double>& secondParameter) const noexcept {
    return arma::norm(secondParameter - firstParameter, "inf");
  }

  arma::Col<double> InfinityNorm::getNeighourImplementation(
      const arma::Col<double>& parameter,
      const arma::Col<double>& minimalDistance,
      const arma::Col<double>& maximalDistance) const noexcept {
    // TODO implement
  }
}
