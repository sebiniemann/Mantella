#include "mantella_bits/numericalAnalysis.hpp"

// C++ standard library
#include <cmath>

namespace mant {
  double gaussianRadialBasisFunctionValue(
      const double distance,
      const double exponent) {
    return std::exp(-std::pow(exponent * distance, 2.0));
  }

  double multiquadraticRadialBasisFunctionValue(
      const double distance,
      const double exponent) {
    return std::sqrt(1 + std::pow(exponent * distance, 2.0));
  }

  double polyharmonicSplineRadialBasisFunctionValue(
      const double distance,
      const arma::uword polynomialOrder) {
    const double absoluteDistance = std::abs(distance);
    if (polynomialOrder % 2 == 0) {
      // Generates a polyharmonic spline for even ..
      return std::pow(absoluteDistance, polynomialOrder) * std::log(absoluteDistance);
    } else {
      // ... and a thin plate spline for uneven polynomial orders.
      return std::pow(absoluteDistance, polynomialOrder);
    }
  }
}
