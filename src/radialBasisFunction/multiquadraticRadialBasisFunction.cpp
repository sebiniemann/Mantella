#include "mantella_bits/radialBasisFunction/multiquadraticRadialBasisFunction.hpp"

// C++ standard library
#include <cmath>

namespace mant {
  MultiquadraticRadialBasisFunction::MultiquadraticRadialBasisFunction(
      const arma::uword numberOfDimensions,
      const double exponent)
      : RadialBasisFunction(numberOfDimensions),
        exponent_(exponent) {
  }

  double MultiquadraticRadialBasisFunction::getBasisFunctionValueImplementation(
      const double distance) const {
    return std::sqrt(std::pow(distance, 2.0) + std::pow(exponent_, 2.0));
  }

  double MultiquadraticRadialBasisFunction::getPolynomialTailValueImplementation(
      const arma::Col<double>& parameter,
      const arma::Col<double>& polynomialCoefficients) const {
    return polynomialCoefficients(polynomialCoefficients.n_elem - 1);
  }

  std::string MultiquadraticRadialBasisFunction::toString() const {
    return "multiquadratic_radial_basis_function";
  }
}
