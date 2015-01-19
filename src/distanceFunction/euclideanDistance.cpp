#include <mantella_bits/distanceFunction/euclideanDistance.hpp>

// Mantella
#include <mantella_bits/helper/rng.hpp>

namespace mant {
  double EuclideanDistance::getDistanceImplementation(
      const arma::Col<double>& parameter) const noexcept {
    return arma::norm(parameter);
  }

  arma::Col<double> EuclideanDistance::getNeighbourImplementation(
      const arma::Col<double>& parameter,
      const double& minimalDistance,
      const double& maximalDistance) const noexcept {
    const arma::Col<double>& velocity = arma::normalise(arma::randn<arma::Col<double>>(parameter.n_elem)) * std::uniform_real_distribution<double>(0.0, 1.0)(Rng::generator);
    return parameter + arma::sign(velocity) * minimalDistance + (maximalDistance - minimalDistance) * velocity;
  }
}
