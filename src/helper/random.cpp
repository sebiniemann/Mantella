#include "mantella_bits/helper/random.hpp"

// C++ standard library
#include <algorithm>
#include <random>

// Mantella
#include "mantella_bits/helper/assert.hpp"
#include "mantella_bits/helper/rng.hpp"

namespace mant {
  arma::Mat<double> getRandomRotationMatrix(
      const arma::uword numberOfDimensions) {
    arma::Mat<double> Q;
    arma::Mat<double> R;

    if (arma::qr(Q, R, arma::randn<arma::Mat<double>>(numberOfDimensions, numberOfDimensions))) {
      return Q * arma::sign(arma::diagmat(R));
    } else {
      const arma::Col<double>& angles = arma::datum::pi * arma::randu<arma::Col<double>>(numberOfDimensions);
      const arma::Col<double>& sineAngles = arma::sin(angles);
      const arma::Col<double>& cosineAngles = arma::cos(angles);

      arma::Mat<double> rotationMatrix = arma::eye<arma::Mat<double>>(numberOfDimensions, numberOfDimensions);
      for (arma::uword n = 0; n < angles.n_elem; ++n) {
        arma::Mat<double> subRotationMatrix = arma::eye<arma::Mat<double>>(numberOfDimensions, numberOfDimensions);
        subRotationMatrix(0, 0) = sineAngles(n);
        subRotationMatrix(0, n) = cosineAngles(n);
        subRotationMatrix(n, 0) = cosineAngles(n);
        subRotationMatrix(n, n) = -sineAngles(n);

        rotationMatrix *= subRotationMatrix;
      }

      return rotationMatrix;
    }
  }

  arma::Col<arma::uword> getRandomPermutation(
      const arma::uword numberOfElements,
      const arma::uword cycleSize) {
    verify(cycleSize <= numberOfElements, "The cycle size must be lower than or equal to the number of elements.");

    arma::Col<arma::uword> permutation(numberOfElements);
    for (arma::uword n = 0; n < numberOfElements; ++n) {
      permutation(n) = n;
    }

    arma::uword length = std::min(cycleSize, numberOfElements - 1);
    for (arma::uword n = 0; n < length; ++n) {
      permutation.swap_rows(n, std::uniform_int_distribution<arma::uword>(n, length)(Rng::getGenerator()));
    }

    return permutation.head(cycleSize);
  }

  arma::Col<arma::uword> getRandomPermutation(
      const arma::uword numberOfElements) {
    return getRandomPermutation(numberOfElements, numberOfElements);
  }
}
