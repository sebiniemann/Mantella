#include "mantella_bits/distanceFunction.hpp"

// Mantella
#include "mantella_bits/helper/assert.hpp"

namespace mant {
  DistanceFunction::DistanceFunction(
      const arma::uword numberOfDimensions)
      : numberOfDimensions_(numberOfDimensions) {
  }

  double DistanceFunction::getDistance(
      const arma::Col<double>& firstParameter,
      const arma::Col<double>& secondParameter) const {
    verify(firstParameter.n_elem == secondParameter.n_elem, ""); // TODO

    return getLengthImplementation(secondParameter - firstParameter);
  }

  double DistanceFunction::getLength(
      const arma::Col<double>& parameter) const {
    return getLengthImplementation(parameter);
  }
}
