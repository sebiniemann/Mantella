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
      explicit SurrogateModel(
          std::unordered_map<arma::Col<double>, double, Hash, IsEqual> samples);
      
      virtual bool isFirstOrderDifferentiable() const;
      virtual bool isSecondOrderDifferentiable() const;
      
      void setLowerBounds(
        const arma::Col<double>& lowerBounds);
      void setUpperBounds(
        const arma::Col<double>& upperBounds);

      arma::Col<double> getLowerBounds() const;
      arma::Col<double> getUpperBounds() const;
      
      void model();
      
      double getFirstOrderDerivative(
          const arma::Col<double>& parameter) const;
      
      double getSecondOrderDerivative(
          const arma::Col<double>& parameter) const;
      
      double getObjectiveValue(
          const arma::Col<double>& parameter) const;
        
      virtual ~SurrogateModel() = default;
      
    protected:
      std::unordered_map<arma::Col<double>, double, Hash, IsEqual> samples_;
      
      arma::Col<double> lowerBounds_;
      arma::Col<double> upperBounds_;
    
      virtual void modelImplementation() = 0;
      
      virtual double getFirstOrderDerivativeImplementation(
          const arma::Col<double>& parameter) const;
      
      virtual double getSecondOrderDerivativeImplementation(
          const arma::Col<double>& parameter) const;
      
      virtual double getObjectiveValueImplementation(
          const arma::Col<double>& parameter) const = 0;
    
  };
}