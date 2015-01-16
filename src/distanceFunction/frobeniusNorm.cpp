#include <mantella_bits/distanceFunction/frobeniusNorm.hpp>

// Mantella
#include <mantella_bits/helper/rng.hpp>

namespace mant {
  double FrobeniusNorm::getDistanceImplementation(
      const arma::Col<double>& firstParameter,
      const arma::Col<double>& secondParameter) const noexcept {
    return arma::norm(secondParameter - firstParameter, "fro");
  }

  arma::Col<double> FrobeniusNorm::getNeighourImplementation(
      const arma::Col<double>& parameter,
      const arma::Col<double>& minimalDistance,
      const arma::Col<double>& maximalDistance) const noexcept {
    // TODO implement
  }
}
