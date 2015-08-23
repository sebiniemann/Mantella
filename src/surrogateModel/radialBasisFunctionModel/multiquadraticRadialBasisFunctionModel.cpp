#include <mantella_bits/surrogateModel/radialBasisFunctionModel/multiquadraticRadialBasisFunctionModel.hpp>

// C++ standard library
#include <cmath>

namespace mant {
  MultiquadraticRadialBasisFunctionModel::MultiquadraticRadialBasisFunctionModel(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples,
      const double exponent) 
    : RadialBasisFunctionModel(samples),
      exponent_(exponent) {
      
  }
  
  double MultiquadraticRadialBasisFunctionModel::getBasisFunctionValue(
      const double distance) const {
    return std::sqrt(std::pow(distance, 2.0) + std::pow(exponent_, 2.0));
  }
  
  double MultiquadraticRadialBasisFunctionModel::getPolynomialTailValue(
      const arma::Col<double>& parameter) const {
    return tail_(tail_.n_elem - 1);
  }
  
  std::string MultiquadraticRadialBasisFunctionModel::toString() const {
    return "multiquadratic_radial_basis_function_model";
  }
}