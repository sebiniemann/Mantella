#pragma once

// C++ standard library
#include <string>

// Mantella
#include <mantella_bits/surrogateModel.hpp>

namespace mant {
  class LinearFunctionModel : public SurrogateModel {
    public:
      const arma::Col<double> coefficients_;
      const double errorTerm_;
    
      explicit LinearFunctionModel(
          const arma::Col<double>& coefficients,
          const double errorTerm);
    
      std::string toString() const override;
  
    protected:
      void modelImplementation() override;
      
      arma::Col<double> getJacobianImplementation(
          const arma::Col<double>& parameter) const override;
      
      double getObjectiveValueImplementation(
          const arma::Col<double>& parameter) const override;
  };
}