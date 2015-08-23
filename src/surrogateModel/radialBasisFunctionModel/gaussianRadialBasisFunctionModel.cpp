#include <mantella_bits/surrogateModel/radialBasisFunctionModel/gaussianRadialBasisFunctionModel.hpp>

// C++ standard library
#include <cmath>

namespace mant {
  GaussianRadialBasisFunctionModel::GaussianRadialBasisFunctionModel(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples,
      const double exponent) 
    : RadialBasisFunctionModel(samples),
      exponent_(exponent) {
      
  }
  
  double GaussianRadialBasisFunctionModel::getBasisFunctionValue(
      const double distance) const {
    return std::exp(-exponent_ * std::pow(distance, 2.0));
  }
  
  double GaussianRadialBasisFunctionModel::getPolynomialTailValue(
      const arma::Col<double>& parameter) const {
    return 0;
  }
  
  std::string GaussianRadialBasisFunctionModel::toString() const {
    return "gaussian_radial_basis_function_model";
  }
}