#include <hop_bits/helper/random.hpp>

#include <cstdlib>
#include <random>
#include <algorithm>

namespace hop {
  decltype(Random::RandomDevice) Random::RandomDevice;
  decltype(Random::Rng) Random::Rng(Random::RandomDevice());

  arma::Mat<double> Random::getRandomRotationMatrix(unsigned int numberOfDimensions) {
    arma::Mat<double> rotationMatrix = arma::randn<arma::Mat<double>>(numberOfDimensions, numberOfDimensions);
    for (std::size_t j = 0; j < rotationMatrix.n_cols; ++j) {
      for (unsigned int jj = 0; jj < j; ++jj) {
        rotationMatrix.col(j) = rotationMatrix.col(j) - arma::dot(rotationMatrix.col(j), rotationMatrix.col(jj)) * rotationMatrix.col(jj);
      }
      rotationMatrix.col(j) = rotationMatrix.col(j) / arma::norm(rotationMatrix.col(j));
    }

    return rotationMatrix;
  }

  arma::Col<arma::uword> Random::getRandomPermutation(unsigned int numberOfElements, unsigned int cycleLength) {
    arma::Col<arma::uword> permutation(numberOfElements);
    for(std::size_t n = 0; n < numberOfElements; ++n) {
      permutation.at(n) = n;
    }

    unsigned int length = std::min(cycleLength, numberOfElements - 1);
    for(std::size_t n = 0; n < length; ++n) {
      permutation.swap_rows(n, std::uniform_int_distribution<unsigned int>(n, length)(Random::Rng));
    }

    return permutation.subvec(0, cycleLength - 1);
  }

  arma::Col<arma::uword> Random::getRandomPermutation(unsigned int numberOfElements) {
    return getRandomPermutation(numberOfElements, numberOfElements);
  }
}
