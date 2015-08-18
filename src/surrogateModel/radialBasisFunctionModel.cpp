#include <mantella_bits/surrogateModel/radialBasisFunctionModel.hpp>

namespace mant {
  void RadialBasisFunctionModel::modelImplementation() {
  
  }
  
  double RadialBasisFunctionModel::getObjectiveValueImplementation(
      const arma::Col<double>& parameter) const {
    return 0.0;
  }
  
  std::string RadialBasisFunctionModel::toString() const {
    return "radial_basis_function";
  }
}