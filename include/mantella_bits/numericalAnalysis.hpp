#pragma once

// Armadillo
#include <armadillo>

namespace mant {
  double multiquadraticRadialBasisFunctionValue(
      const double distance,
      const double exponent);

  double gaussianRadialBasisFunctionValue(
      const double distance,
      const double exponent);

  double polyharmonicSplineRadialBasisFunctionValue(
      const double distance,
      const arma::uword polynomialOrder);
}
