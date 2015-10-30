#pragma once

// C++ standard library
#include <string>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella_bits/correlationFunction.hpp>

namespace mant {
  class PolynomialCorrelationFunction : public CorrelationFunction {
   public:
    arma::uword polynomialOrder_;

    explicit PolynomialCorrelationFunction(
        const arma::uword numberOfDimensions,
        const arma::uword polynomialOrder);

    std::string toString() const override;

   protected:
    double getCorrelationCoefficientImplementation(
        const arma::Col<double>& parameter) const override;
  };
}
