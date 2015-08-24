#pragma once

// C++ standard library
#include <unordered_map>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella_bits/helper/unorderedContainer.hpp>
#include <mantella_bits/surrogateModel.hpp>

namespace mant {
  class RadialBasisFunctionModel : public SurrogateModel {
    public:
      explicit RadialBasisFunctionModel(
          const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples);
  
      virtual ~RadialBasisFunctionModel() = default;
      
    protected:
      arma::Col<double> coefficients_;
      arma::Col<double> tail_;
    
      void modelImplementation() final;
      
      virtual double getBasisFunctionValue(
          const double distance) const = 0;
      
      virtual double getPolynomialTailValue(
          const arma::Col<double>& parameter) const = 0;
      
      double getObjectiveValueImplementation(
          const arma::Col<double>& parameter) const final;
  };
}