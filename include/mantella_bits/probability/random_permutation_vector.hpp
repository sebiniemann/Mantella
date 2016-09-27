#pragma once

/**

*/
  arma::uvec randomPermutationVector(
      const arma::uword numberOfElements,
      const arma::uword cycleSize) {
    assert(numberOfElements > 1 && "randomRotationMatrix: The number of elements must be greater than 0.");
    assert(cycleSize > 1 && "randomRotationMatrix: The cycle size must be greater than 0.");
    assert(cycleSize <= numberOfElements && "randomRotationMatrix: The cycle size must be lower than or equal to the number of elements.");

    // Generates a random permutation, based on an in-place Fisher-Yates-shuffle.
    arma::uvec permutation(arma::regspace<arma::uvec>(0, numberOfElements - 1));
    for (arma::uword n = 0; n < cycleSize; ++n) {
      permutation.swap_rows(n, std::uniform_int_distribution<arma::uword>(n, permutation.n_elem - 1)(Rng::getGenerator()));
    }

    return permutation.head(cycleSize);
  }