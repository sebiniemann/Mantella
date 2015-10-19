#pragma once

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/helper/printable.hpp"

namespace mant {
  class RegressionFunction : public Printable {
   public:
    const arma::uword numberOfParameterDimensions_;
    const arma::uword numberOfRegressionDimensions_;

    explicit RegressionFunction(
        const arma::uword numberOfParameterDimensions,
        const arma::uword numberOfRegressionDimensions);

    arma::Col<double> getRegression(
        const arma::Col<double>& parameter) const;

    virtual ~RegressionFunction() = default;

   protected:
    virtual arma::Col<double> getRegressionImplementation(
        const arma::Col<double>& parameter) const = 0;
  };
}
