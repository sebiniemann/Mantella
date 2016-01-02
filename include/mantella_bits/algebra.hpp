#pragma once

// Armadillo
#include <armadillo>

namespace mant {
  arma::uword polynomialSize(
      const arma::uword numberOfElements,
      const arma::uword polynomialOrder);

  arma::Col<double> polynomial(
      const arma::Col<double>& parameter,
      const arma::uword polynomialOrder);
}
