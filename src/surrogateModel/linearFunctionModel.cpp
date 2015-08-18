#include <mantella_bits/surrogateModel/linearFunctionModel.hpp>

namespace mant {
  void LinearFunctionModel::modelImplementation() {
  
  }
  
  double LinearFunctionModel::getObjectiveValueImplementation(
      const arma::Col<double>& parameter) const {
    return 0.0;
  }
  
  std::string LinearFunctionModel::toString() const {
    return "linear_function_model";
  }
}