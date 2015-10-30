#include "mantella_bits/helper/setTheory.hpp"

// C++ standard library
#include <algorithm>
#include <cmath>
#include <iterator>

// Mantella
#include "mantella_bits/helper/assert.hpp"

namespace mant {
  std::vector<arma::Col<arma::uword>> getCombinations(
      const arma::uword numberOfElements,
      const arma::uword combinationSize) {
    verify(combinationSize <= numberOfElements, ""); // TODO

    std::vector<arma::Col<arma::uword>> combinations;

    std::vector<arma::uword> bitmask(numberOfElements);
    std::fill(bitmask.begin(), std::next(bitmask.begin(), static_cast<decltype(bitmask)::difference_type>(combinationSize)), 1);
    do {
      arma::Col<arma::uword> combination(combinationSize);
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

  std::vector<std::pair<arma::Col<arma::uword>, arma::Col<arma::uword>>> getTwoSetsPartitions(
      const arma::uword numberOfElements) {
    arma::Col<arma::uword> elements = arma::linspace<arma::Col<arma::uword>>(0, numberOfElements - 1, numberOfElements);

    std::vector<std::pair<arma::Col<arma::uword>, arma::Col<arma::uword>>> partitions;

    for (arma::uword n = 1; n <= std::floor(static_cast<double>(numberOfElements) / 2.0); ++n) {
      arma::Col<arma::uword> firstSet = elements.head(n);
      arma::Col<arma::uword> secondSet = elements.tail(elements.n_elem - n);

      partitions.push_back({firstSet, secondSet});

      arma::Col<arma::uword> counter(n, arma::fill::zeros);
      arma::uword shiftedIndex = counter.n_elem;

      arma::uword lastIndex = 0;
      if (firstSet.n_elem == secondSet.size()) {
        lastIndex = 1;
      }

      while (shiftedIndex > lastIndex) {
        const arma::uword index = shiftedIndex - 1;

        std::iter_swap(firstSet.begin() + index, secondSet.begin() + counter(index));
        partitions.push_back({firstSet, secondSet});

        if (counter(index) < secondSet.n_elem - 1) {
          counter.tail(counter.n_elem - index).fill(counter(index) + 1);
          shiftedIndex = counter.n_elem;
        } else {
          --shiftedIndex;
        }
      }
    }

    return partitions;
  }
}
