#include <mantella_bits/surrogateModel/quadraticFunctionModel.hpp>

namespace mant {
  void QuadraticFunctionModel::modelImplementation() {
  
  }
  
  double QuadraticFunctionModel::getObjectiveValueImplementation(
      const arma::Col<double>& parameter) const {
    return 0.0;
  }
  
  std::string QuadraticFunctionModel::toString() const {
    return "quadratic_function_model";
  }
}