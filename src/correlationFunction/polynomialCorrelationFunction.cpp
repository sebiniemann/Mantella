#include <mantella_bits/correlationFunction/polynomialCorrelationFunction.hpp>

// C++ standard library
#include <cmath>
#include <stdexcept>

namespace mant {
  PolynomialCorrelationFunction::PolynomialCorrelationFunction(
      const arma::uword numberOfDimensions,
      const arma::uword polynomialOrder)
    : CorrelationFunction(numberOfDimensions),
     polynomialOrder_(polynomialOrder) {
     
  }

  double PolynomialCorrelationFunction::getCorrelationCoefficientImplementation(
      const arma::Col<double>& parameter) const {
    const arma::Col<double>& boundedParameter = arma::min(arma::abs(parameter), arma::ones(numberOfDimensions_));
    double correlationCoefficient = 0.0;
    
    switch (polynomialOrder_) {
      case 1:
        correlationCoefficient = arma::prod(boundedParameter);
        break;
      case 2:
        correlationCoefficient = arma::prod(1 - boundedParameter % (1.5 - 0.5 * arma::pow(boundedParameter, 2.0)));
        break;
      case 3:
        correlationCoefficient = arma::prod(1 - arma::pow(boundedParameter, 2.0) % (3 - 2 * boundedParameter));
        break;
    }
    
    return correlationCoefficient;
  }

  std::string PolynomialCorrelationFunction::toString() const {
    return "polynomial_correlation_function";
  }
}
