#include <mantella_bits/surrogateModel/quadraticFunctionModel.hpp>

// Mantella
#include <mantella_bits/propertyAnalysis/passivePropertyAnalysis/quadraticFunctionModelAnalysis.hpp>

namespace mant {
  void QuadraticFunctionModel::modelImplementation() {
    QuadraticFunctionModelAnalysis quadraticFunctionModelAnalysis(samples_);
    quadraticFunctionModelAnalysis.analyse();
    
    quadraticCoefficients_ = quadraticFunctionModelAnalysis.getQuadraticCoefficients();
    linearCoefficients_ = quadraticFunctionModelAnalysis.getLinearCoefficients();
    errorTerm_ = quadraticFunctionModelAnalysis.getErrorTerm();
    
    hessian_ = quadraticCoefficients_ + quadraticCoefficients_.t();
  }
      
  arma::Col<double> QuadraticFunctionModel::getJacobianImplementation(
      const arma::Col<double>& parameter) const {
    return hessian_ * parameter;
  }

  arma::Mat<double> QuadraticFunctionModel::getHessianImplementation(
      const arma::Col<double>& parameter) const {
    return hessian_;
  }
  
  double QuadraticFunctionModel::getObjectiveValueImplementation(
      const arma::Col<double>& parameter) const {
    return arma::dot(parameter.t() * quadraticCoefficients_, parameter) + arma::dot(parameter, linearCoefficients_) + errorTerm_;
  }
  
  std::string QuadraticFunctionModel::toString() const {
    return "quadratic_function_model";
  }
}