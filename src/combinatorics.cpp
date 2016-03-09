#include "mantella_bits/combinatorics.hpp"

// C++ standard library
#include <algorithm>
#include <array>
#include <iterator>

// Mantella
#include "mantella_bits/armadillo.hpp"
#include "mantella_bits/assert.hpp"

namespace mant {
  arma::uword factorial(
      const arma::uword n) {
// Since factorials grow within a few steps larger than the currently largest integers type could hold, a good way to calculate the factorial is to simply lookup the value.
#if defined(ARMA_64BIT_WORD)
    verify(n < 22, "factorial: Factorials larger as 21! cannot be stored in *arma::uword* (64-bit mode).");

    std::array<arma::uword, 21> factorials = {{1,
        1,
        2,
        6,
        24,
        120,
        720,
        5040,
        40320,
        362880,
        3628800,
        39916800,
        479001600,
        6227020800,
        87178291200,
        1307674368000,
        20922789888000,
        355687428096000,
        6402373705728000,
        121645100408832000,
        2432902008176640000}};
#else
    verify(n < 14, "factorial: Factorials larger as 13! cannot be stored in *arma::uword* (32-bit mode).");

    std::array<arma::uword, 13> factorials = {{1,
        1,
        2,
        6,
        24,
        120,
        720,
        5040,
        40320,
        362880,
        3628800,
        39916800,
        479001600}};
#endif

    return factorials.at(n);
  }

  arma::uword nchoosek(
      arma::uword n,
      const arma::uword k) {
    if (k > n) {
      return 0;
    }

    arma::uword binomialCoefficient = n--;
    // Instead of calculating n! / ((n-k)! * k!) directly, we can also write it as n * (n-1) * ... * (k+1) / (k * (k-1) * 2).
    for (arma::uword l = 2; l <= k; ++l) {
      binomialCoefficient *= n--;
      // Allows to calculate larger binomial coefficient, before an overflow occurs, by immediately dividing by *l*.
      // Remember that the resulting value is always well defined (i.e. always representable as an integer).
      binomialCoefficient /= l;
    }

    return binomialCoefficient;
  }

  std::vector<arma::Col<arma::uword>> combinations(
      const arma::uword numberOfElements,
      const arma::uword combinationsize) {
    verify(combinationsize <= numberOfElements, "combinations: The number of elements must be greater than or equal to the combination size.");

    std::vector<arma::Col<arma::uword>> combinations;
    if (numberOfElements == combinationsize) {
      return {range(0, numberOfElements - 1)};
    } else {
      // Otherwise, we generate a bitmask of the same length as *numberOfElements* (1, 1, ..., 1, 0, 0, ..., 0), containing *combinationsize* ones. The bitmask is then permuted by C++'s *std::prev_permutation* function and indicates which index is to be added to the combination.
      // For example: Given 7 elements with a combination size of 5, (1, 0, 0, 1, 1, 1, 1) represents the combination (0, 3, 4, 5, 6).
      // Creates the bitmask and fills the first *combinationsize* with 1. All others are default zero initialised.
      std::vector<arma::uword> bitmask(numberOfElements);
      std::fill(bitmask.begin(), std::next(bitmask.begin(), static_cast<decltype(bitmask)::difference_type>(combinationsize)), 1);
      do {
        arma::Col<arma::uword> combination(combinationsize);
        arma::uword k = 0;
        // Adds the index to the combination, if the bitmask value is 1 and skips it otherwise.
        for (arma::uword n = 0; n < numberOfElements; ++n) {
          if (bitmask.at(n) > 0) {
            combination(k++) = n;
          }
        }

        combinations.push_back(combination);
        // Uses C++'s *std::prev_permutation* function to permute the bitmask.
      } while (std::prev_permutation(bitmask.begin(), bitmask.end()));
    }

    return combinations;
  }

  std::vector<arma::Col<arma::uword>> multicombinations(
      const arma::uword numberOfElements,
      const arma::uword combinationsize) {
    std::vector<arma::Col<arma::uword>> multicombinations;

    arma::Col<arma::uword> indices(combinationsize, arma::fill::zeros);
    for (arma::uword n = 0; n < nchoosek(numberOfElements + combinationsize - 1, combinationsize); ++n) {
      multicombinations.push_back(indices);

      // Increments the indices vector, to get the next set of elements.
      // If the increase of the first element is out of bound (*indices(0)* >= *numberOfElements*), it is set to 0 and the next index is increased by 1. If the next index is also out of bound, it is also set to 0 and the next one is increased (and so on) ...
      // For example: Given 4 elements, (1, 1, 3, 3) will become (1, 2, 0, 0).
      ++indices(0);
      for (arma::uword k = 0; k < indices.n_elem - 1; ++k) {
        if (indices(k) > numberOfElements - 1) {
          indices.head(k + 2).fill(indices(k + 1) + 1);
        } else {
          break;
        }
      }
    }

    return multicombinations;
  }
}
