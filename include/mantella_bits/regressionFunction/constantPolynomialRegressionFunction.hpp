#pragma once

// C++ standard library
#include <string>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella_bits/regressionFunction.hpp>

namespace mant {
  class ConstantRegressionFunction : public RegressionFunction {
    public:
      explicit ConstantRegressionFunction(
          const arma::uword numberOfParameterDimensions);
    
      std::string toString() const override;
    
    protected:
      arma::Col<double> getRegressionImplementation(
          const arma::Col<double>& parameter) const override;
  };
}
