#pragma once

// C++ standard library
#include <string>

// Armdillo
#include <armadillo>

// Mantella
#include <mantella_bits/helper/unorderedContainer.hpp>
#include <mantella_bits/optimisationProblem/surrogateModel.hpp>
#include <mantella_bits/regressionFunction.hpp>

namespace mant {
  class PolynomialFunctionModel : public SurrogateModel {
    public:
      const std::shared_ptr<RegressionFunction> regressionFunction_;
    
      explicit PolynomialFunctionModel(
          const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples,
          const std::shared_ptr<RegressionFunction> regressionFunction);
      
      std::string toString() const;
      
    protected:
      arma::Col<double> coefficients_;
    
      void modelImplementation() override;
      
      double getObjectiveValueImplementation(
          const arma::Col<double>& parameter) const override;
  };
}
