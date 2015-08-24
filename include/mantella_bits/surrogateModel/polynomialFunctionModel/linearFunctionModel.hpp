#pragma once

// C++ standard library
#include <string>

// Mantella
#include <mantella_bits/surrogateModel.hpp>

namespace mant {
  class LinearFunctionModel : public SurrogateModel {
    public:
      using SurrogateModel::SurrogateModel;
    
    
    
      std::string toString() const override;
  
    protected:
      arma::Col<double> coefficients_;
      double errorTerm_;
      
      void modelImplementation() override;
      
      arma::Col<double> getJacobianImplementation(
          const arma::Col<double>& parameter) const override;
      
      double getObjectiveValueImplementation(
          const arma::Col<double>& parameter) const override;
  };
}