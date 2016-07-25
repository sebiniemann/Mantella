#include "mantella_bits/combinatorics.hpp"

// C++ standard library
#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <iterator>
#include <limits>
#include <stdexcept>
#include <string>

// Mantella
#include "mantella_bits/assert.hpp"

namespace mant {
  arma::uword factorial(
      const arma::uword n) {
// Since factorials grow within a few steps larger than the largest number representable by `arma::uword`, a good way to *calculate* the factorial is to simply lookup its value.
#if defined(ARMA_64BIT_WORD)
    std::array<arma::uword, 21> factorials = {{1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800, 39916800, 479001600, 6227020800, 87178291200, 1307674368000, 20922789888000, 355687428096000, 6402373705728000, 121645100408832000, 2432902008176640000}};
#else
    std::array<arma::uword, 13> factorials = {{1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800, 39916800, 479001600}};
#endif
    if (n >= factorials.size()) {
      throw std::overflow_error("factorial: Factorials larger as " + std::to_string(factorials.size()) + "! will be greater than the largest supported integer.");
    }

    return factorials.at(n);
  }

  arma::uword nchoosek(
      arma::uword n,
      const arma::uword k) {
    if (k > n) {
      return 0;
    } else if (k == 0 || n == k) {
      return 1;
    } else if (k == 1) {
      return n;
    }

    /* Instead of calculating
     *
     *      n!
     * -------------
     * ((n-k)! * k!)
     * 
     * directly, we can also write it as
     * 
     * /     / / / n * (n-1) \         \         \     \
     * |     | | | --------- | * (n-2) |         |     |
     * |     | | \     2     /         | * (n-3) |     |
     * | ... | | --------------------- |         | ... |
     * |     | \           3           /         |     |  * (k+1)
     * |     | --------------------------------- |     |
     * |     \                 4                 /     |
     * \                         ...                   /
     * ----------------------------------------------------------
     *                              k
     *
     * This avoids temporarily storing large integers and allows us to calculate greater binomial coefficients, before hitting an overflow.
     * In fact, the largest number we are temporarily storing by using this method is exactly `k` times larger than the resulting binomial coefficient.
     * **Note:** The dividends are always divisible, since when multiplying `n` consecutive numbers, one is a multiply of `n`.
     */

    arma::uword binomialCoefficient = n;
    for (arma::uword l = 2; l <= k; ++l) {
      // **Note:** Since the results of the left term will be floored, we might be stricter than we have to be, but will not miss an overflow.
      if ((std::numeric_limits<decltype(binomialCoefficient)>::max() / (n - 1)) <
          binomialCoefficient) {
        throw std::overflow_error("nchoosek: The binomialCoefficient will be greater than the largest supported integer.");
      }

      binomialCoefficient *= --n;
      binomialCoefficient /= l;
    }

    return binomialCoefficient;
  }

  arma::uword secondStirlingNumber(
      const arma::uword numberOfElements,
      const arma::uword numberOfParts) {
    if (numberOfElements == 0 || numberOfParts == 0 || numberOfElements < numberOfParts) {
      return 0;
    } else if (numberOfParts == 1 || numberOfElements == numberOfParts) {
      return 1;
    }

    /* Instead of calculating the second Stirling number directly based on its explicit formula
     *
     *  1     k               / k \
     * --- * sum (-1)^(k-l) * |   | * l^n
     *  k!   l=1              \ l /
     *
     * we wrote it as
     *
     *  k                   l^(n-1)
     * sum (-1)^(k-l) * ---------------
     * l=1              (l-1)! * (k-l)!
     *
     * This avoids temporarily storing large integers and allows us to calculate greater Stirling numbers, before hitting an overflow.
     * However, the fractions are not representable as an integer in most cases, wherefore we perform the whole computation based on floating points.
     */

    // The largest values we are converting later on are actually reduced by 1.
    if (!isRepresentableAsFloatingPoint(numberOfElements - 1)) {
      throw std::range_error("secondStirlingNumber: The number of elements must be representable as a floating point.");
    } else if (!isRepresentableAsFloatingPoint(numberOfParts - 1)) {
      throw std::range_error("secondStirlingNumber: The number of parts must be representable as a floating point.");
    }

    double secondStirlingNumber = 0;
    for (arma::uword l = 1; l <= numberOfParts; ++l) {
      try {
        secondStirlingNumber += std::pow(-1.0, static_cast<decltype(secondStirlingNumber)>(numberOfParts - l)) * std::pow(l, (static_cast<decltype(secondStirlingNumber)>(numberOfElements - 1))) / (factorial(l - 1) * factorial(numberOfParts - l));
      } catch (const std::overflow_error& exception) {
        throw std::overflow_error("secondStirlingNumber: The second Stirling number will be greater than the largest supported integer.");
      }
    }
    // Fixes rounding errors due to the limited precision.
    secondStirlingNumber = std::round(secondStirlingNumber);

    if (!isRepresentableAsInteger(secondStirlingNumber)) {
      throw std::overflow_error("secondStirlingNumber: The second Stirling number will be greater than the largest supported integer.");
    }

    return static_cast<arma::uword>(secondStirlingNumber);
  }

  std::vector<arma::uvec> combinations(
      const arma::uword numberOfElements,
      const arma::uword cycleSize) {
    assert(numberOfElements > 0 && "combinations: The combination size must be greater than 0.");
    assert(numberOfElements <= cycleSize && "combinations: The number of elements must be greater than or equal to the combination size.");

    if (cycleSize == 0) {
      return {};
    }
    if (numberOfElements == cycleSize) {
      return {arma::regspace<arma::uvec>(0, numberOfElements - 1)};
    } else {
      /* Generates all combinations by permuting a bit mask of the same length as `numberOfElements`, having `cycleSize` 1's for each index being part of the current combination and 0's otherwise.
       * If `numberOfElements` was set to 7 and `cycleSize` to 4, we would get:
       * 
       * (1, 1, 1, 1, 0, 0, 0) Bit mask (initial)
       *  |  |  |  |
       * (0, 1, 2, 3         ) Combination
       *
       *
       * (1, 1, 1, 0, 1, 0, 0) Bit mask (permuted in reverse lexicographic)
       *  |  |  |     |
       * (0, 1, 2,    4      ) Combination
       *
       *
       * (1, 1, 1, 0, 0, 1, 0) Bit mask (permuted in reverse lexicographic)
       *  |  |  |        |
       * (0, 1, 2,       5   ) Combination
       *
       *          ...
       *
       * (0, 0, 0, 1, 1, 1, 1) Bit mask (permuted in reverse lexicographic)
       *           |  |  |  |
       * (         3, 4, 5, 6) Combination
       */

      // **Note:** The C++ standard guarantees that the vector initialises all its elements to 0, wherefore we are only setting the first `cycleSize` elements to 1.
      std::vector<arma::uword> bitmask(numberOfElements);

      if (cycleSize > std::numeric_limits<decltype(bitmask)::difference_type>::max()) {
        throw std::range_error("combinations: The combination size must be less than or equal to the largest representable iterator difference.");
      }
      std::fill(bitmask.begin(), std::next(bitmask.begin(), static_cast<decltype(bitmask)::difference_type>(cycleSize)), 1);

      std::vector<arma::uvec> combinations;
      combinations.reserve(nchoosek(numberOfElements, cycleSize));

      do {
        // Converts the bit mask to a combination.
        arma::uvec combination(cycleSize);
        arma::uword k = 0;
        for (arma::uword n = 0; n < numberOfElements; ++n) {
          if (bitmask.at(n) > 0) {
            combination(k++) = n;
          }
        }

        combinations.push_back(combination);
      } while (std::prev_permutation(bitmask.begin(), bitmask.end()));

      return combinations;
    }
  }

  std::vector<arma::uvec> multicombinations(
      const arma::uword numberOfElements,
      const arma::uword cycleSize) {
    assert(numberOfElements > 0 && "multicombinations: The number of elements must be greater than 0.");

    if (cycleSize == 0) {
      return {};
    }

    /* Iterates through all `numberOfElements`-adic numbers having `cycleSize` digits, skipping all number whose values are not monotonically decreasing from the less to the most significant digit.
     * If `numberOfElements` was set to 3 and `cycleSize` to 4, we would get:
     *
     * (0, 0, 0, 0)
     * (1, 0, 0, 0)
     * (2, 0, 0, 0)
     * (1, 1, 0, 0)
     * (2, 1, 0, 0)
     * (2, 2, 0, 0)
     * (1, 1, 1, 0)
     *     ...
     * (2, 2, 2, 2)
     */

    // Temporarily stores the number of multi-combinations, as C++ does not guarantee that `.capacity()` will return the same value as requested by `.reserve(...)`.
    const arma::uword numberOfMulticombinations = nchoosek(numberOfElements + cycleSize - 1, cycleSize);
    std::vector<arma::uvec> multicombinations;
    multicombinations.reserve(numberOfMulticombinations);

    arma::uvec indices(cycleSize, arma::fill::zeros);
    for (arma::uword n = 0; n < numberOfMulticombinations; ++n) {
      multicombinations.push_back(indices);
      ++indices(0);

      // Increments the next (more significant) digit by 1 if we overshoot the maximal value for a `numberOfElements`-adic number.
      // All less significant digits are set to the same value, as we are skipping numbers whose values are not monotonically decreasing from the less to the most significant digit.
      for (arma::uword k = 0; k < indices.n_elem - 1; ++k) {
        if (indices(k) == numberOfElements) {
          indices.head(k + 1).fill(++indices(k + 1));
        } else {
          break;
        }
      }
    }

    return multicombinations;
  }

  std::vector<std::pair<arma::uvec, arma::uvec>> twoSetsPartitions(
      const arma::uword numberOfElements) {
    assert(numberOfElements > 1 && "twoSetsPartitions: The number of elements must be greater than 1.");

    if (numberOfElements == 2) {
      return {{{0}, {1}}};
    }

    /* Generates all partitions with two sets by permuting a bit mask of the same length as `numberOfElements`, having *n* 1's for each index being part of the first part and 0's for the second one. *n* denotes the amount of elements that is distributed to each part and iterated up to `numberOfElements` / 2, as we would add duplicates (since there is no order between being the first or second part of a partition) of previously added permutations for greater values.
     * If `numberOfElements` was set to 4, we would get:
     * 
     * n = 1:
     *   (1, 0, 0, 0) Bit mask (initial)
     *    |
     *   (0         ) First part  \
     *       |  |  |               |- First partition
     *   (   1  2  3) Second part /
     *
     *
     *   (1, 0, 0, 0) Bit mask (permuted in reverse lexicographic)
     *       |
     *   (   1      ) First part  \
     *    |     |  |               |- Second partition
     *   (0     2  3) Second part /
     *
     *   ...
     * 
     * n = 2:
     *   (1, 1, 0, 0) Bit mask (initial)
     *    |  |
     *   (0, 1      ) First part  \
     *          |  |               |- Fifth partition
     *   (      2  3) Second part /
     *
     *
     *   (1, 0, 1, 0) Bit mask (permuted in reverse lexicographic)
     *    |     |
     *   (0,    2   ) First part  \
     *       |     |               |- Sixth partition
     *   (   1     3) Second part /
     *
     *   ...
     */

    // Checks that the later `static_cast<decltype(bitmask)::difference_type>(n)` cast is safe for all given `n`.
    if (numberOfElements / 2 > std::numeric_limits<std::vector<arma::uword>::difference_type>::max()) {
      throw std::range_error("twoSetsPartitions: The halved number of elements must be less than or equal to the largest representable iterator difference.");
    }

    std::vector<std::pair<arma::uvec, arma::uvec>> partitions;
    partitions.reserve(secondStirlingNumber(numberOfElements, 2));

    for (arma::uword n = 1; n <= numberOfElements / 2; ++n) {
      // **Note:** The C++ standard guarantees that the vector initialises all its elements to 0, wherefore we are only setting the first `n` elements to 1.
      std::vector<arma::uword> bitmask(numberOfElements);
      std::fill(bitmask.begin(), std::next(bitmask.begin(), static_cast<decltype(bitmask)::difference_type>(n)), 1);
      do {
        /* Avoids adding duplicates when we split the elements in equally sized
         * parts, skipping the second half of permutations in this case.
         * If `numberOfElements` was set to 4, we would get:
         * * n = 2
         * (1, 1, 0, 0) (Bit mask)
         * (1, 0, 1, 0) (Bit mask)
         * (1, 0, 0, 1) (Bit mask)
         * (0, 1, 1, 0) (Bit mask) Invert of the first bit mask <- Skipped from here on
         * (0, 1, 0, 1) (Bit mask) Invert of the second bit mask
         * (0, 0, 1, 1) (Bit mask) Invert of the third bit mask
         */
        if (2 * n == numberOfElements && bitmask.at(0) != 1) {
          break;
        }

        partitions.push_back({arma::regspace<arma::uvec>(0, numberOfElements - 1).elem(arma::find(arma::uvec(bitmask) == 1)), arma::regspace<arma::uvec>(0, numberOfElements - 1).elem(arma::find(arma::uvec(bitmask) == 0))});
      } while (std::prev_permutation(bitmask.begin(), bitmask.end()));
    }

    return partitions;
  }
}
