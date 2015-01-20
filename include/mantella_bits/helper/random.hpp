#pragma once

// Armadillo
#include <armadillo>

namespace mant {
  arma::Mat<double> getRandomRotationMatrix(
      const unsigned int& numberOfDimensions) noexcept;

  arma::Col<unsigned int> getRandomPermutation(
      const unsigned int& numberOfElements,
      const unsigned int& cycleLength);

  arma::Col<unsigned int> getRandomPermutation(
      const unsigned int& numberOfElements) noexcept;
}
