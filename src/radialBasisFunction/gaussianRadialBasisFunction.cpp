#include "mantella_bits/radialBasisFunction/gaussianRadialBasisFunction.hpp"

// C++ standard library
#include <cmath>

namespace mant {
  GaussianRadialBasisFunction::GaussianRadialBasisFunction(
      const arma::uword numberOfDimensions,
      const double exponent)
      : RadialBasisFunction(numberOfDimensions),
        exponent_(exponent) {
  }

  double GaussianRadialBasisFunction::getBasisFunctionValueImplementation(
      const double distance) const {
    return std::exp(-exponent_ * std::pow(distance, 2.0));
  }

  double GaussianRadialBasisFunction::getPolynomialTailValueImplementation(
      const arma::Col<double>& parameter,
      const arma::Col<double>& polynomialCoefficients) const {
    return 0;
  }

  std::string GaussianRadialBasisFunction::toString() const {
    return "gaussian_radial_basis_function";
  }
}
