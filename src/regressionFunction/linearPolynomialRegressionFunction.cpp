#include <mantella_bits/regressionFunction/linearPolynomialRegressionFunction.hpp>

namespace mant {
  LinearRegressionFunction::LinearRegressionFunction(
      const arma::uword numberOfParameterDimensions)
    : RegressionFunction(numberOfParameterDimensions, numberOfParameterDimensions + 1) {
    
  }

  arma::Col<double> LinearRegressionFunction::getRegressionImplementation(
      const arma::Col<double>& parameter) const {
    arma::Col<double> regression(numberOfRegressionDimensions_);

    regression.head(numberOfParameterDimensions_) = parameter;
    regression(numberOfParameterDimensions_) = 1;
    
    return regression;
  }
  
  std::string LinearRegressionFunction::toString() const {
    return "linear_regression_function";
  }
}
