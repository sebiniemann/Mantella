#pragma once

// C++ standard library
#include <unordered_map>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella_bits/helper/printable.hpp>
#include <mantella_bits/helper/unorderedContainer.hpp>

namespace mant {
  class SurrogateModel : public Printable {
    public:
      const arma::uword numberOfDimensions_;
    
      explicit SurrogateModel(
          const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples);
      
      void setLowerBounds(
        const arma::Col<double>& lowerBounds);
      void setUpperBounds(
        const arma::Col<double>& upperBounds);

      arma::Col<double> getLowerBounds() const;
      arma::Col<double> getUpperBounds() const;
      
      void model();
      
      arma::Col<double> getJacobian(
          const arma::Col<double>& parameter) const;
      
      arma::Mat<double> getHessian(
          const arma::Col<double>& parameter) const;
      
      double getObjectiveValue(
          const arma::Col<double>& parameter) const;
        
      virtual ~SurrogateModel() = default;
      
    protected:
      std::unordered_map<arma::Col<double>, double, Hash, IsEqual> samples_;
      
      arma::Col<double> lowerBounds_;
      arma::Col<double> upperBounds_;
    
      virtual void modelImplementation() = 0;
      
      virtual arma::Col<double> getJacobianImplementation(
          const arma::Col<double>& parameter) const;
      
      virtual arma::Mat<double> getHessianImplementation(
          const arma::Col<double>& parameter) const;
      
      virtual double getObjectiveValueImplementation(
          const arma::Col<double>& parameter) const = 0;
    
  };
}