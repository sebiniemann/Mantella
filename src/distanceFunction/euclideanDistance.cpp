#include <mantella_bits/distanceFunction/euclideanDistance.hpp>

// Mantella
#include <mantella_bits/helper/rng.hpp>

namespace mant {
  double EuclideanDistance::getDistanceImplementation(
      const arma::Col<double>& firstParameter,
      const arma::Col<double>& secondParameter) const noexcept {
    return arma::norm(secondParameter - firstParameter);
  }

  arma::Col<double> EuclideanDistance::getNeighourImplementation(
      const arma::Col<double>& parameter,
      const arma::Col<double>& minimalDistance,
      const arma::Col<double>& maximalDistance) const noexcept {
    const arma::Col<double>& velocity = arma::normalise(arma::randn<arma::Col<double>>(parameter.n_elem)) * std::uniform_real_distribution<double>(0.0, 1.0)(Rng::generator);
    return parameter + arma::sign(velocity) % minimalDistance + (maximalDistance - minimalDistance) % velocity;
  }
}
