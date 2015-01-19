#include <mantella_bits/distanceFunction/infinityNorm.hpp>

// Mantella
#include <mantella_bits/helper/rng.hpp>

namespace mant {
  double InfinityNorm::getDistanceImplementation(
      const arma::Col<double>& parameter) const noexcept {
    return arma::norm(parameter, "inf");
  }

  arma::Col<double> InfinityNorm::getNeighbourImplementation(
      const arma::Col<double>& parameter,
      const double& minimalDistance,
      const double& maximalDistance) const noexcept {
    const arma::Col<double>& velocity = 2.0 * arma::randu<arma::Col<double>>(parameter.n_elem) - 1.0;
    return parameter + arma::sign(velocity) * minimalDistance + (maximalDistance - minimalDistance) * velocity;
  }
}
