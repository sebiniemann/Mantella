#pragma once

// Armadillo
#include <armadillo>

namespace mant {
  arma::uword polynomialSize(
      const arma::uword numberOfIndeterminates,
      const arma::uword highestDegree);

  arma::vec polynomial(
      const arma::vec& parameter,
      const arma::uword highestDegree);
}
