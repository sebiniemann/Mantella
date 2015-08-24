#pragma once

// C++ standard library
#include <string>

// Mantella
#include <mantella_bits/surrogateModel.hpp>

namespace mant {
  class QuadraticFunctionModel : public SurrogateModel {
    public:
      using SurrogateModel::SurrogateModel;
  
      std::string toString() const override;
  
    protected:
      arma::Mat<double> quadraticCoefficients_;
      arma::Col<double> linearCoefficients_;
      double errorTerm_;
      arma::Mat<double> hessian_;
    
      void modelImplementation() override;
      
      arma::Col<double> getJacobianImplementation(
          const arma::Col<double>& parameter) const override;
  
      arma::Mat<double> getHessianImplementation(
          const arma::Col<double>& parameter) const override;
      
      double getObjectiveValueImplementation(
          const arma::Col<double>& parameter) const override;
  };
}