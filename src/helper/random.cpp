#include <mantella_bits/helper/random.hpp>

// C++ Standard Library
#include <cstdlib>
#include <algorithm>

// Mantella
#include <mantella_bits/helper/rng.hpp>

namespace mant {
  arma::Mat<double> getRandomRotationMatrix(
      const unsigned int& numberOfDimensions) noexcept {
    arma::Mat<double> Q;
    arma::Mat<double> R;
    arma::qr(Q, R, arma::randn<arma::Mat<double>>(numberOfDimensions, numberOfDimensions));
    return Q * arma::sign(arma::diagmat(R));
  }

  arma::Col<unsigned int> getRandomPermutation(
      const unsigned int& numberOfElements,
      const unsigned int& cycleSize) noexcept {
    arma::Col<unsigned int> permutation(numberOfElements);
    for (std::size_t n = 0; n < numberOfElements; ++n) {
      permutation.at(n) = n;
    }

    unsigned int length = std::min(cycleSize, numberOfElements - 1);
    for (std::size_t n = 0; n < length; ++n) {
      permutation.swap_rows(n, std::uniform_int_distribution<unsigned int>(n, length)(Rng::generator));
    }

    return permutation.head(cycleSize);
  }

  arma::Col<unsigned int> getRandomPermutation(
      const unsigned int& numberOfElements) noexcept {
    return getRandomPermutation(numberOfElements, numberOfElements);
  }
}
