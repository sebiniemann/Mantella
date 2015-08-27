#pragma once

// C++ standard library
#include <unordered_map>
#include <string>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella_bits/helper/unorderedContainer.hpp>
#include <mantella_bits/radialBasisFunction.hpp>

namespace mant {
  class MultiquadraticRadialBasisFunction : public RadialBasisFunction {
    public:
      const double exponent_;
      
      explicit MultiquadraticRadialBasisFunction(
          const arma::uword numberOfDimensions,
          const double exponent);

      std::string toString() const override;
  
    protected:
      double getBasisFunctionValueImplementation(
          const double distance) const override;
      
      double getPolynomialTailValueImplementation(
          const arma::Col<double>& parameter,
      const arma::Col<double>& polynomialCoefficients) const override;
  };
}
