#include "mantella_bits/radialBasisFunction.hpp"

namespace mant {
  RadialBasisFunction::RadialBasisFunction(
      const arma::uword numberOfDimensions)
      : numberOfDimensions_(numberOfDimensions) {
  }

  double RadialBasisFunction::getBasisFunctionValue(
      const double distance) const {
    return getBasisFunctionValueImplementation(distance);
  }

  double RadialBasisFunction::getPolynomialTailValue(
      const arma::Col<double>& parameter,
      const arma::Col<double>& polynomialCoefficients) const {
    return getPolynomialTailValueImplementation(parameter, polynomialCoefficients);
  }
}
