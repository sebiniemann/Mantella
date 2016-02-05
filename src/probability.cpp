#include "mantella_bits/probability.hpp"

// C++ standard library
#include <cassert>
#include <random>

// Mantella
#include "mantella_bits/armadillo.hpp"
#include "mantella_bits/assert.hpp"
#include "mantella_bits/randomNumberGenerator.hpp"

namespace mant {
  arma::Mat<double> randomRotationMatrix(
      const arma::uword numberOfDimensions) {
    arma::Mat<double> Q;
    arma::Mat<double> R;

    // Generating a randomly and uniformly rotation matrix based on the properties of the QR decomposition.
    arma::qr(Q, R, arma::randn<arma::Mat<double>>(numberOfDimensions, numberOfDimensions));

    // Asserts that *Q* is an orthogonal matrix.
    assert(arma::any(arma::vectorise(arma::abs(arma::pinv(Q).t() - Q)) <= 1e-12));
    // Asserts that *R* is an upper triangular matrix.
    assert(arma::all(arma::vectorise(R == arma::trimatu(R))));

    return Q * arma::sign(arma::diagmat(R));
  }

  arma::Col<arma::uword> randomPermutationVector(
      const arma::uword numberOfElements,
      const arma::uword cycleSize) {
    verify(cycleSize <= numberOfElements, "randomPermutationVector: The cycle size must be lower than or equal to the number of elements.");

    arma::Col<arma::uword> permutation = range(0, numberOfElements - 1);

    // Generates a random permutation, based on the in-place Fisher-Yates-shuffle.
    for (arma::uword n = 0; n < cycleSize; ++n) {
      permutation.swap_rows(n, std::uniform_int_distribution<arma::uword>(n, permutation.n_elem - 1)(Rng::getGenerator()));
    }

    return permutation.head(cycleSize);
  }

  arma::Col<arma::uword> randomPermutationVector(
      const arma::uword numberOfElements) {
    return randomPermutationVector(numberOfElements, numberOfElements);
  }

  arma::Col<double> randomNeighbour(
      const arma::Col<double>& parameter,
      const double minimalDistance,
      const double maximalDistance) {
    verify(minimalDistance >= 0, "randomNeighbour: The minimal distance must be positive (including 0).");
    verify(minimalDistance <= maximalDistance, "randomNeighbour: The minimal distance must be less than or equal to the maximal one.");

    return parameter + arma::normalise(arma::randn<arma::Col<double>>(parameter.n_elem)) * (minimalDistance + std::uniform_real_distribution<double>(0, 1)(Rng::getGenerator()) * (maximalDistance - minimalDistance));
  }
}
