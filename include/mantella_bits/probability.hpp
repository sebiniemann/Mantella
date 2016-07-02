#pragma once

// Armadillo
#include <armadillo>

namespace mant {
  arma::mat randomRotationMatrix(
      const arma::uword numberOfDimensions);

  arma::uvec randomPermutationVector(
      const arma::uword numberOfElements,
      const arma::uword cycleSize);

  arma::vec randomNeighbour(
      const arma::vec& parameter,
      const double minimalDistance,
      const double maximalDistance);
}
