#include "mantella_bits/probability.hpp"

// C++ standard library
#include <cassert>

// Mantella
#include "mantella_bits/randomNumberGenerator.hpp"

namespace mant {
  arma::mat randomRotationMatrix(
      const arma::uword numberOfDimensions) {
    assert(numberOfDimensions > 1 && "randomRotationMatrix: The number of dimensions must be greater than 1.");

    // @see Francesco Mezzadri (2007). How to Generate Random Matrices from the Classical Compact Groups. Notices of the AMS, 54(5), pp. 592-604.
    arma::mat Q;
    arma::mat R;
    if (arma::qr(Q, R, normalRandomNumbers(numberOfDimensions, numberOfDimensions))) {
      return Q * arma::sign(arma::diagmat(R));
    } else {
      // In very rare cases, the QR decomposition fails due to the limited numerical precision. In these rare cases, we simply return an identity matrix.
      return arma::eye<arma::mat>(numberOfDimensions, numberOfDimensions);
    }
  }

  arma::uvec randomPermutationVector(
      const arma::uword numberOfElements,
      const arma::uword cycleSize) {
    assert(numberOfElements > 1 && "randomRotationMatrix: The number of elements must be greater than 0.");
    assert(cycleSize > 1 && "randomRotationMatrix: The cycle size must be greater than 0.");
    assert(cycleSize <= numberOfElements && "randomRotationMatrix: The cycle size must be lower than or equal to the number of elements.");

    // Generates a random permutation, based on an in-place Fisher-Yates-shuffle.
    arma::uvec permutation = arma::regspace<arma::uvec>(0, numberOfElements - 1);
    for (arma::uword n = 0; n < cycleSize; ++n) {
      permutation.swap_rows(n, std::uniform_int_distribution<arma::uword>(n, permutation.n_elem - 1)(Rng::getGenerator()));
    }

    return permutation.head(cycleSize);
  }

  arma::uvec randomPermutationVector(
      const arma::uword numberOfElements) {
    return randomPermutationVector(numberOfElements, numberOfElements);
  }

  arma::vec randomNeighbour(
      const arma::vec& parameter,
      const double minimalDistance,
      const double maximalDistance) {
    assert(!parameter.is_empty() && "randomNeighbour: The parameter must be non-empty.");
    assert(!parameter.has_nan() && "randomNeighbour: The parameter must not have NaNs.");
    assert(minimalDistance >= 0 && "randomNeighbour: The minimal distance must be positive (including 0).");
    assert(minimalDistance <= maximalDistance && "randomNeighbour: The minimal distance must be less than or equal to the maximal one.");

    // @see J. S. Hicks and R. F. Wheeling (1959). An efficient method for generating uniformly distributed points on the surface of an n-dimensional sphere. Communications of the ACM, 2(4), pp. 17-19.
    return parameter + arma::normalise(normalRandomNumbers(parameter.n_rows, parameter.n_cols)) * (minimalDistance + std::uniform_real_distribution<double>(0, 1)(Rng::getGenerator()) * (maximalDistance - minimalDistance));
  }

  arma::vec randomNeighbour(
      const arma::vec& parameter,
      const double maximalDistance) {
    return randomNeighbour(parameter, 0.0, maximalDistance);
  }

  arma::mat uniformRandomNumbers(
      const arma::uword numberOfRows,
      const arma::uword numberOfColumns,
      std::uniform_real_distribution<double> distribution) {
    arma::mat randomValues(numberOfRows, numberOfColumns);
    randomValues.imbue([&distribution]() { return distribution(Rng::getGenerator()); });
    return randomValues;
  }

  arma::mat uniformRandomNumbers(
      const arma::uword numberOfRows,
      const arma::uword numberOfColumns) {
    return uniformRandomNumbers(numberOfRows, numberOfColumns, std::uniform_real_distribution<double>());
  }

  arma::vec uniformRandomNumbers(
      const arma::uword numberOfElements,
      std::uniform_real_distribution<double> distribution) {
    return uniformRandomNumbers(numberOfElements, 1, distribution);
  }

  arma::vec uniformRandomNumbers(
      const arma::uword numberOfElements) {
    return uniformRandomNumbers(numberOfElements, 1);
  }

  arma::mat normalRandomNumbers(
      const arma::uword numberOfRows,
      const arma::uword numberOfColumns,
      std::normal_distribution<double> distribution) {
    arma::mat randomValues(numberOfRows, numberOfColumns);
    randomValues.imbue([&distribution]() { return distribution(Rng::getGenerator()); });
    return randomValues;
  }

  arma::mat normalRandomNumbers(
      const arma::uword numberOfRows,
      const arma::uword numberOfColumns) {
    return normalRandomNumbers(numberOfRows, numberOfColumns, std::normal_distribution<double>());
  }

  arma::vec normalRandomNumbers(
      const arma::uword numberOfElements,
      std::normal_distribution<double> distribution) {
    return normalRandomNumbers(numberOfElements, 1, distribution);
  }

  arma::vec normalRandomNumbers(
      const arma::uword numberOfElements) {
    return normalRandomNumbers(numberOfElements, 1);
  }
}
