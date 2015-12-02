#pragma once

// Armadillo
#include <armadillo>

namespace mant {
  arma::Mat<double> randomRotationMatrix(
      const arma::uword numberOfDimensions);

  arma::Col<arma::uword> randomPermutationMatrix(
      const arma::uword numberOfElements,
      const arma::uword cycleSize);

  arma::Col<arma::uword> randomPermutationMatrix(
      const arma::uword numberOfElements);
      
  arma::Col<double> randomNeighbour(
      const arma::Col<double>& parameter,
      const double minimalDistance,
      const double maximalDistance);
}
