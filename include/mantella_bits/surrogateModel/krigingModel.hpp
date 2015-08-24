#pragma once

// C++ standard library
#include <string>
#include <unordered_map>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella_bits/helper/unorderedContainer.hpp>
#include <mantella_bits/surrogateModel.hpp>

namespace mant {
  class KrigingModel : public SurrogateModel {
    public:
      const arma::uword polynomialOrder_;
    
      KrigingModel(
          const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples,
          const arma::uword polynomialOrder);
          
    protected:
      arma::Col<double> meanParameter_;
      arma::Col<double> standardDeviationParameter_;
      
      double meanObjectiveValue_;
      double standardDeviationObjectiveValue_;
      
      arma::Col<double> beta_;
      arma::Col<double> gamma_;
    
      void modelImplementation() override;
      
      arma::Col<double> getNthOrderRegression(
          const arma::Col<double>& parameter) const;
          
      virtual double getCorrelationCoefficient(
          const arma::Col<double>& parameter) const = 0;
      
      double getObjectiveValueImplementation(
          const arma::Col<double>& parameter) const final;
  };
}