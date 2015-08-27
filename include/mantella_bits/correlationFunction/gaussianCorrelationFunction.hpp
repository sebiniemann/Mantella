#pragma once

// C++ standard library
#include <string>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella_bits/correlationFunction.hpp>

namespace mant {
  class GaussianCorrelationFunction : public CorrelationFunction {
    public:
      using CorrelationFunction::CorrelationFunction;
      
      std::string toString() const override;
      
    protected:
      double getCorrelationCoefficientImplementation(
          const arma::Col<double>& parameter) const override;
  };
}
