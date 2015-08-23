#pragma once

// C++ standard library
#include <unordered_map>
#include <string>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella_bits/helper/unorderedContainer.hpp>
#include <mantella_bits/surrogateModel/radialBasisFunctionModel.hpp>

namespace mant {
  class PolyharmonicSplineRadialBasisFunctionModel : public RadialBasisFunctionModel {
    public:
      const arma::uword exponent_;
      
      explicit PolyharmonicSplineRadialBasisFunctionModel(
          const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples,
          const arma::uword exponent);

      std::string toString() const override;
  
    protected:
      double getBasisFunctionValue(
          const double distance) const override;
      
      double getPolynomialTailValue(
          const arma::Col<double>& parameter) const override;
  };
}