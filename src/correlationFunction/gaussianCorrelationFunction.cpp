#include <mantella_bits/correlationFunction/gaussianCorrelationFunction.hpp>

// C++ standard library
#include <cmath>

namespace mant {
  double GaussianCorrelationFunction::getCorrelationCoefficientImplementation(
      const arma::Col<double>& parameter) const {
    return arma::prod(std::exp(arma::accu(-arma::square(parameter))));
  }

  std::string GaussianCorrelationFunction::toString() const {
    return "gaussian_correlation_function";
  }
}
