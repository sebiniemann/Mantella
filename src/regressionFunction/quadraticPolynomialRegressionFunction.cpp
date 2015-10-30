#include "mantella_bits/regressionFunction/quadraticPolynomialRegressionFunction.hpp"

namespace mant {
  QuadraticRegressionFunction::QuadraticRegressionFunction(
      const arma::uword numberOfParameterDimensions)
      : RegressionFunction(numberOfParameterDimensions, numberOfParameterDimensions * (numberOfParameterDimensions + 1) / 2 + numberOfParameterDimensions + 1) {
  }

  arma::Col<double> QuadraticRegressionFunction::getRegressionImplementation(
      const arma::Col<double>& parameter) const {
    arma::Col<double> regression(numberOfRegressionDimensions_);

    arma::uword n = 0;
    for (arma::uword k = 0; k < numberOfParameterDimensions_; ++k) {
      for (arma::uword l = k; l < numberOfParameterDimensions_; ++l) {
        regression(n++) = parameter(k) * parameter(l);
      }
    }

    regression.subvec(n, n + numberOfParameterDimensions_ - 1) = parameter;
    n += numberOfParameterDimensions_;

    regression(n) = 1;

    return regression;
  }

  std::string QuadraticRegressionFunction::toString() const {
    return "quadratic_regression_function";
  }
}
