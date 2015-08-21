#include <mantella_bits/surrogateModel/linearFunctionModel.hpp>


namespace mant {
  void LinearFunctionModel::modelImplementation() {
    // Nothing to do here
  }
      
  arma::Col<double> LinearFunctionModel::getJacobianImplementation(
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