#pragma once

// C++ standard library
#include <unordered_map>

// Armadillo
#include <armadillo>
#include <string>

// Mantella
#include <mantella_bits/helper/unorderedContainer.hpp>
#include <mantella_bits/optimisationProblem/surrogateModel.hpp>
#include <mantella_bits/radialBasisFunction.hpp>

namespace mant {
  class RadialBasisFunctionModel : public SurrogateModel {
    public:
      const std::shared_ptr<RadialBasisFunction> radialBasisFunction_;
    
      explicit RadialBasisFunctionModel(
          const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples,
          const std::shared_ptr<RadialBasisFunction> radialBasisFunction);
      
      std::string toString() const;
      
    protected:
      arma::Col<double> coefficients_;
      arma::Col<double> tail_;
    
      void modelImplementation() override;
      
      double getObjectiveValueImplementation(
          const arma::Col<double>& parameter) const override;
  };
}
