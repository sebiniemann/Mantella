#pragma once

// C++ standard library
#include <string>

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/radialBasisFunction.hpp"

namespace mant {
  class PolyharmonicSplineRadialBasisFunction : public RadialBasisFunction {
   public:
    const arma::uword polynomialOrder_;

    explicit PolyharmonicSplineRadialBasisFunction(
        const arma::uword numberOfDimensions,
        const arma::uword polynomialOrder);

    std::string toString() const override;

   protected:
    double getBasisFunctionValueImplementation(
        const double distance) const override;

    double getPolynomialTailValueImplementation(
        const arma::Col<double>& parameter,
        const arma::Col<double>& polynomialCoefficients) const override;
  };
}
