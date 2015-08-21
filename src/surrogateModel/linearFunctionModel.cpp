#include <mantella_bits/surrogateModel/linearFunctionModel.hpp>

namespace mant {
  LinearFunctionModel::LinearFunctionModel(
      const arma::Col<double>& coefficients,
      const double errorTerm)
    : SurrogateModel(true, false),
      coefficients_(coefficients),
      errorTerm_(errorTerm) {
    
  }

  void LinearFunctionModel::modelImplementation() {
    // Nothing to do here
  }
      
  double LinearFunctionModel::getJacobian(
      const arma::Col<double>& parameter) const {
    return coefficients_;
  }
  
  double LinearFunctionModel::getObjectiveValueImplementation(
      const arma::Col<double>& parameter) const {
    return arma::dot(parameter, coefficients_) + errorTerm_;
  }
  
  std::string LinearFunctionModel::toString() const {
    return "linear_function_model";
  }
}