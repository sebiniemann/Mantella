#pragma once

// Armadillo
#include <armadillo>

namespace hop {
  arma::Mat<double> getRandomRotationMatrix(
      unsigned int numberOfDimensions);

  arma::Col<arma::uword> getRandomPermutation(
      unsigned int numberOfElements,
      unsigned int cycleLength);

  arma::Col<arma::uword> getRandomPermutation(
      unsigned int numberOfElements);
}
