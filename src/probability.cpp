#include "mantella_bits/probability.hpp"

// C++ standard library
#include <algorithm>
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
    assert(arma::any(arma::vectorise(arma::abs(arma::pinv(Q).t() - Q)) <= 1.0e-12));
    // Asserts that *R* is an upper triangular matrix.
    assert(arma::all(arma::vectorise(R == arma::trimatu(R))));

    return Q * arma::sign(arma::diagmat(R));
  }

  arma::Col<arma::uword> randomPermutationVector(
      const arma::uword numberOfElements,
      const arma::uword cycleSize) {
    verify(cycleSize <= numberOfElements, "randomPermutationVector: The cycle size must be lower than or equal to the number of elements.");

    arma::Col<arma::uword> permutation = range<arma::uword>(0, numberOfElements - 1);

    // Generates a random permutation, based on the in-place Fisher-Yates-shuffle.
    arma::uword length = std::min(cycleSize, numberOfElements - 1);
    for (arma::uword n = 0; n < cycleSize; ++n) {
      permutation.swap_rows(n, std::uniform_int_distribution<arma::uword>(n, length)(Rng::getGenerator()));
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
    verify(minimalDistance >= 0, "randomNeighbour: "); // TODO
    verify(minimalDistance <= maximalDistance, "randomNeighbour: "); // TODO

    arma::Col<double> displacement = arma::normalise(arma::randn<arma::Col<double>>(parameter.n_elem)) * (maximalDistance - minimalDistance);
    displacement += arma::sign(displacement) * minimalDistance;

    if (maximalDistance > 0 && arma::all(displacement == 0)) {
      displacement(std::uniform_int_distribution<arma::uword>(0, parameter.n_elem - 1)(Rng::getGenerator())) = (std::bernoulli_distribution(0.5)(Rng::getGenerator()) ? 1.0 : -1.0) * maximalDistance;
    }

    return parameter + displacement;
  }
}
