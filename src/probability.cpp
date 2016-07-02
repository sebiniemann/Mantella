#include "mantella_bits/probability.hpp"

// C++ standard library
#include <random>
#include <stdexcept>

// Mantella
#include "mantella_bits/randomNumberGenerator.hpp"

namespace mant {
  arma::mat randomRotationMatrix(
      const arma::uword numberOfDimensions) {
    if (numberOfDimensions < 2) {
      throw std::domain_error("randomRotationMatrix: The number of dimensions must be greater than 1.");
    }

    // @see Francesco Mezzadri (2007). How to Generate Random Matrices from the Classical Compact Groups. Notices of the AMS, 54(5), pp. 592-604.
    arma::mat Q;
    arma::mat R;
    if (arma::qr(Q, R, arma::randn<arma::mat>(numberOfDimensions, numberOfDimensions))) {
      return Q * arma::sign(arma::diagmat(R));
    } else {
      // In very rare cases, the QR decomposition fails due to the limited numerical precision. In these rare cases, we simply return an identity matrix.
      return arma::eye<arma::mat>(numberOfDimensions, numberOfDimensions);
    }
  }

  arma::uvec randomPermutationVector(
      const arma::uword numberOfElements,
      const arma::uword cycleSize) {
    if (numberOfElements == 0) {
      throw std::domain_error("randomPermutationVector: The number of elements must be greater than 0.");
    } else if (cycleSize == 0) {
      throw std::domain_error("randomPermutationVector: The cycle size must be greater than 0.");
    } else if (cycleSize > numberOfElements) {
      throw std::logic_error("randomPermutationVector: The cycle size must be lower than or equal to the number of elements.");
    }

    // Generates a random permutation, based on an in-place Fisher-Yates-shuffle.
    arma::uvec permutation = arma::regspace<arma::uvec>(0, numberOfElements - 1);
    for (arma::uword n = 0; n < cycleSize; ++n) {
      permutation.swap_rows(n, std::uniform_int_distribution<arma::uword>(n, permutation.n_elem - 1)(Rng::generator_));
    }

    return permutation.head(cycleSize);
  }

  arma::vec randomNeighbour(
      const arma::vec& parameter,
      const double minimalDistance,
      const double maximalDistance) {
    if (parameter.is_empty()) {
      throw std::invalid_argument("randomNeighbour: The parameter must not be empty.");
    } else if (minimalDistance < 0) {
      throw std::domain_error("randomNeighbour: The minimal distance must be positive (including 0).");
    } else if (minimalDistance > maximalDistance) {
      throw std::logic_error("randomNeighbour: The minimal distance must be less than or equal to the maximal one.");
    }

    // @see J. S. Hicks and R. F. Wheeling (1959). An efficient method for generating uniformly distributed points on the surface of an n-dimensional sphere. Communications of the ACM, 2(4), pp. 17-19.
    return parameter + arma::normalise(arma::randn<arma::vec>(arma::size(parameter))) * (minimalDistance + std::uniform_real_distribution<double>(0, 1)(Rng::generator_) * (maximalDistance - minimalDistance));
  }
}
