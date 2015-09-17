#pragma once

// Armadillo
#include <armadillo>

namespace mant {
  arma::Mat<double> getHaltonSequence(
      const arma::Col<arma::uword>& base,
      const arma::Col<arma::uword>& seed,
      const arma::uword numberOfColumms);

  arma::Col<double> getVanDerCorputSequence(
      const arma::uword base,
      const arma::uword seed,
      const arma::uword numberOfColumms);
}
