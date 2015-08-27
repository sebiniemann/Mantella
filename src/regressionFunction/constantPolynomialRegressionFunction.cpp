#include <mantella_bits/regressionFunction/constantPolynomialRegressionFunction.hpp>

namespace mant {
  ConstantRegressionFunction::ConstantRegressionFunction(
      const arma::uword numberOfParameterDimensions)
    : RegressionFunction(numberOfParameterDimensions, 1) {
    
  }

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
  arma::Col<double> ConstantRegressionFunction::getRegressionImplementation(
      const arma::Col<double>& parameter) const {
    return {1.0};
  }
#pragma clang diagnostic pop
  
  std::string ConstantRegressionFunction::toString() const {
    return "constant_regression_function";
  }
}
