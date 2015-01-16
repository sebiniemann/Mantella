#include <mantella_bits/distanceFunction/manhattanDistance.hpp>

// Mantella
#include <mantella_bits/helper/rng.hpp>

namespace mant {
  double ManhattanDistance::getDistanceImplementation(
      const arma::Col<double>& firstParameter,
      const arma::Col<double>& secondParameter) const noexcept {
    return arma::norm(secondParameter - firstParameter, 1);
  }

  arma::Col<double> ManhattanDistance::getNeighourImplementation(
      const arma::Col<double>& parameter,
      const arma::Col<double>& minimalDistance,
      const arma::Col<double>& maximalDistance) const noexcept {
    // TODO implement
  }
}
