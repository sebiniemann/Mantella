#pragma once

// Armadillo
#include <armadillo>

namespace hop {
  arma::Mat<double> getRandomRotationMatrix(
      unsigned int numberOfDimensions) noexcept;

  arma::Col<arma::uword> getRandomPermutation(
      unsigned int numberOfElements,
      unsigned int cycleLength) noexcept;

  arma::Col<arma::uword> getRandomPermutation(
      unsigned int numberOfElements) noexcept;
}
