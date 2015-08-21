#pragma once

// C++ standard library
#include <string>

// Mantella
#include <mantella_bits/surrogateModel.hpp>

namespace mant {
  class RadialBasisFunctionModel : public SurrogateModel {
    public:
      explicit RadialBasisFunctionModel(
          const arma::uword numberOfDimensions);
    
      std::string toString() const override;
  
    protected:
      void modelImplementation() override;
      
      double getObjectiveValueImplementation(
          const arma::Col<double>& parameter) const override;
  };
}