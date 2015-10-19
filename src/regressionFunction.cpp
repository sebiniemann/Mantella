#include "mantella_bits/regressionFunction.hpp"

namespace mant {
  RegressionFunction::RegressionFunction(
      const arma::uword numberOfParameterDimensions,
      const arma::uword numberOfRegressionDimensions)
      : numberOfParameterDimensions_(numberOfParameterDimensions),
        numberOfRegressionDimensions_(numberOfRegressionDimensions) {
  }

  arma::Col<double> RegressionFunction::getRegression(
      const arma::Col<double>& parameter) const {
    return getRegressionImplementation(parameter);
  }
}
