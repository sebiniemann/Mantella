#pragma once

// Armadillo
#include <armadillo>

namespace mant {
  arma::Mat<double> getRandomRotationMatrix(
      const arma::uword numberOfDimensions);

  arma::Col<arma::uword> getRandomPermutation(
      const arma::uword numberOfElements,
      const arma::uword cycleSize);

  arma::Col<arma::uword> getRandomPermutation(
      const arma::uword numberOfElements);
}
