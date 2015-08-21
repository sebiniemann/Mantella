#pragma once

// C++ standard library
#include <string>

// Mantella
#include <mantella_bits/surrogateModel.hpp>

namespace mant {
  class QuadraticFunctionModel : public SurrogateModel {
    public:
      const arma::Mat<double> quadraticCoefficients_;
      const arma::Col<double> linearCoefficients_;
      const double errorTerm_;
    
      explicit QuadraticFunctionModel(
          const arma::Mat<double>& quadraticCoefficients,
          const arma::Col<double>& linearCoefficients,
          const double errorTerm);
  
      std::string toString() const override;
  
    protected:
      const arma::Mat<double> hessian_;
    
      void modelImplementation() override;
      
      arma::Col<double> getJacobian(
          const arma::Col<double>& parameter) const override;
  
      arma::Mat<double> getHessian(
          const arma::Col<double>& parameter) const override;
      
      double getObjectiveValueImplementation(
          const arma::Col<double>& parameter) const override;
  };
}