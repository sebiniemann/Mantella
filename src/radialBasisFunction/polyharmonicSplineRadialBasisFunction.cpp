#include <mantella_bits/radialBasisFunction/polyharmonicSplineRadialBasisFunction.hpp>

// C++ standard library
#include <cmath>
#include <cstdlib>

namespace mant {
  PolyharmonicSplineRadialBasisFunction::PolyharmonicSplineRadialBasisFunction(
      const arma::uword numberOfDimensions,
      const arma::uword polynomialOrder) 
    : RadialBasisFunction(numberOfDimensions),
      polynomialOrder_(polynomialOrder) {
      
  }
  
  double PolyharmonicSplineRadialBasisFunction::getBasisFunctionValueImplementation(
      const double distance) const {
    if (polynomialOrder_ % 2 == 0) {
      return std::pow(distance, polynomialOrder_) * std::log(distance);
    } else {
      return std::pow(distance, polynomialOrder_);
    }
  }
  
  double PolyharmonicSplineRadialBasisFunction::getPolynomialTailValueImplementation(
      const arma::Col<double>& parameter,
      const arma::Col<double>& polynomialCoefficients) const {
    if (std::abs(polynomialOrder_) > 1) {
      return arma::dot(polynomialCoefficients.head(numberOfDimensions_), parameter) + polynomialCoefficients(polynomialCoefficients.n_elem - 1);
    } else {
      return polynomialCoefficients(polynomialCoefficients.n_elem - 1);
    }
  }
  
  std::string PolyharmonicSplineRadialBasisFunction::toString() const {
    return "polyharmonic_spline_radial_basis_function";
  }
}
