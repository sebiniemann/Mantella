#include "mantella_bits/correlationFunction/exponentialCorrelationFunction.hpp"

// C++ standard library
#include <cmath>

namespace mant {
  double ExponentialCorrelationFunction::getCorrelationCoefficientImplementation(
      const arma::Col<double>& parameter) const {
    return arma::prod(std::exp(arma::accu(-arma::abs(parameter))));
  }

  std::string ExponentialCorrelationFunction::toString() const {
    return "exponential_correlation_function";
  }
}
