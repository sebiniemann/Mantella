#include <mantella_bits/surrogateModel/radialBasisFunctionModel.hpp>

namespace mant {
  RadialBasisFunctionModel::RadialBasisFunctionModel(
      const arma::uword numberOfDimensions)
    : SurrogateModel(numberOfDimensions, false, false) {
    
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