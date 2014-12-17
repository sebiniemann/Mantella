#include <hop_bits/distanceFunction/euclideanDistance.hpp>

namespace hop {
  double EuclideanDistance::getDistanceImplementation(
      const arma::Col<double>& firstParameter,
      const arma::Col<double>& secondParameter) const {
    return arma::norm(secondParameter - firstParameter);
  }
}
