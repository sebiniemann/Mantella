#include <mantella_bits/surrogateModel/radialBasisFunctionModel/polyharmonicSplineRadialBasisFunctionModel.hpp>

// C++ standard library
#include <cmath>
#include <cstdlib>

namespace mant {
  PolyharmonicSplineRadialBasisFunctionModel::PolyharmonicSplineRadialBasisFunctionModel(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples,
      const arma::uword exponent) 
    : RadialBasisFunctionModel(samples),
      exponent_(exponent) {
      
  }
  
  double PolyharmonicSplineRadialBasisFunctionModel::getBasisFunctionValue(
      const double distance) const {
    if (exponent_ % 2 == 0) {
      return std::pow(distance, exponent_) * std::log(distance);
    } else {
      return std::pow(distance, exponent_);
    }
  }
  
  double PolyharmonicSplineRadialBasisFunctionModel::getPolynomialTailValue(
      const arma::Col<double>& parameter) const {
    if (std::abs(exponent_) > 1) {
      return arma::dot(tail_.head(numberOfDimensions_), parameter) + tail_(tail_.n_elem - 1);
    } else {
      return tail_(tail_.n_elem - 1);
    }
  }
  
  std::string PolyharmonicSplineRadialBasisFunctionModel::toString() const {
    return "polyharmonic_spline_radial_basis_function_model";
  }
}