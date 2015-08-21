#include <mantella_bits/surrogateModel/radialBasisFunctionModel.hpp>

namespace mant {
  RadialBasisFunctionModel::RadialBasisFunctionModel(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples)
    : SurrogateModel(samples) {
    
  }

  void RadialBasisFunctionModel::modelImplementation() {
  
  }
  
  double RadialBasisFunctionModel::getObjectiveValueImplementation(
      const arma::Col<double>& parameter) const {
    return 0.0;
  }
  
  std::string RadialBasisFunctionModel::toString() const {
    return "radial_basis_function_model";
  }
}