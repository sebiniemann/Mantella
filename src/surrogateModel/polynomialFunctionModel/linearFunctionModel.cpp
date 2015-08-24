#include <mantella_bits/surrogateModel/linearFunctionModel.hpp>

// Mantella
#include <mantella_bits/propertyAnalysis/passivePropertyAnalysis/linearFunctionModelAnalysis.hpp>

namespace mant {
  void LinearFunctionModel::modelImplementation() {
      mant::LinearFunctionModelAnalysis linearFunctionModelAnalysis(samples_);
      linearFunctionModelAnalysis.analyse();
      
      coefficients_ = linearFunctionModelAnalysis.getCoefficients();
      errorTerm_ = linearFunctionModelAnalysis.getErrorTerm();
  }
  
  double LinearFunctionModel::getObjectiveValueImplementation(
      const arma::Col<double>& parameter) const {
    return arma::dot(parameter, coefficients_) + errorTerm_;
  }
  
  std::string LinearFunctionModel::toString() const {
    return "linear_function_model";
  }
}