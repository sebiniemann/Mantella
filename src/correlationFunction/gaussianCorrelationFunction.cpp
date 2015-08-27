#include <mantella_bits/correlationFunction/gaussianCorrelationFunction.hpp>

namespace mant {
  double GaussianCorrelationFunction::getCorrelationCoefficientImplementation(
      const arma::Col<double>& parameter) const {
    return arma::prod(std::exp(arma::accu(-arma::square(parameter))));
  }

  std::string GaussianCorrelationFunction::toString() const {
    return "gaussian_correlation_function";
  }
}
