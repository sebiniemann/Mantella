#pragma once

// Armadillo
#include <armadillo>

// Mantella
#include <mantella_bits/helper/printable.hpp>

namespace mant {
  class RadialBasisFunction : public Printable {
   public:
    const arma::uword numberOfDimensions_;

    explicit RadialBasisFunction(
        const arma::uword numberOfDimensions);

    double getBasisFunctionValue(
        const double distance) const;

    double getPolynomialTailValue(
        const arma::Col<double>& parameter,
        const arma::Col<double>& polynomialCoefficients) const;

    ~RadialBasisFunction() = default;

   protected:
    virtual double getBasisFunctionValueImplementation(
        const double distance) const = 0;

    virtual double getPolynomialTailValueImplementation(
        const arma::Col<double>& parameter,
        const arma::Col<double>& polynomialCoefficients) const = 0;
  };
}
