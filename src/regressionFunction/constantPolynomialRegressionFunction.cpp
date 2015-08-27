#include <mantella_bits/regressionFunction/constantPolynomialRegressionFunction.hpp>

namespace mant {
  ConstantRegressionFunction::ConstantRegressionFunction(
      const arma::uword numberOfParameterDimensions)
    : RegressionFunction(numberOfParameterDimensions, 1) {
    
  }

  arma::Col<double> ConstantRegressionFunction::getRegressionImplementation(
      const arma::Col<double>& parameter) const {
    return {1.0};
  }
  
  std::string ConstantRegressionFunction::toString() const {
    return "constant_regression_function";
  }
}
