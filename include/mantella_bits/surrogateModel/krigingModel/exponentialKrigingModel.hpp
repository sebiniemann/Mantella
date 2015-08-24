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
  class ExponentialKrigingModel : public KrigingModel {
    public:
      const arma::Col<double> coefficients_;
    
      ExponentialKrigingModel(
          const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples,
          const arma::Col<double> coefficients);
    
      std::string toString() const override;
      
    protected:
      double getCorrelationCoefficient(
          const arma::Col<double>& parameter) const override; 
  };
}