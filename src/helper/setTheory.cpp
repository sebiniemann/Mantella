#include <mantella_bits/helper/setTheory.hpp>

// C++ standard library
#include <algorithm>

namespace mant {
  std::vector<arma::Col<arma::uword>> getCombinations(
      const arma::uword numberOfElements,
      const arma::uword combinationSize) {
    std::vector<arma::Col<arma::uword>> combinations;
      
    std::vector<int> bitmask(numberOfElements);
    std::fill(bitmask.begin(), bitmask.begin() + combinationSize, 1);
    do {
      arma::Col<arma::uword> combination(combinationSize);
      arma::uword k = 0;
      for (arma::uword n = 0; n < numberOfElements; ++n) {
        if (bitmask.at(n)) {
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

    for(arma::uword n = 1; n <= std::floor(static_cast<double>(numberOfElements) / 2.0); ++n) {
      arma::Col<arma::uword> firstSet = elements.head(n);
      arma::Col<arma::uword> secondSet = elements.tail(elements.n_elem - n);

      partitions.push_back({firstSet, secondSet});

      // TODO Why int?
      arma::Col<arma::uword> counter(n, arma::fill::zeros);
      int index = counter.n_elem - 1;

      int lastIndex = -1;
      if(firstSet.n_elem == secondSet.size()) {
        lastIndex = 0;
      }

      while (index > lastIndex) {
        std::iter_swap(firstSet.begin() + index, secondSet.begin() + counter(index));
        partitions.push_back({firstSet, secondSet});

        if(counter(index) < secondSet.n_elem - 1) {
          counter.tail(counter.n_elem - index).fill(counter(index) + 1);
          index = counter.n_elem - 1;
        } else {
          --index;
        }
      }
    }

    return partitions;
  }
}
