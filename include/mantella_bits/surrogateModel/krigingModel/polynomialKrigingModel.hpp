#pragma once

// C++ standard library
#include <string>
#include <unoredred_map>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella_bits/helper/unorderedContainer.hpp>
#include <mantella_bits/surrogateModel/krigingModel.hpp>

namespace once {
  class PolynomialKrigingModel : public KrigingModel {
    public:
      const arma::uword polynomialOrder_;
      const arma::Col<double> coefficients_;
    
      PolynomialKrigingModel(
          const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples,
          const arma::uword polynomialOrder,
          const arma::Col<double> coefficients);
    
      std::string toString() const override;
      
    protected:
      double getCorrelationCoefficient(
          const arma::Col<double>& parameter) const override; 
  };
}