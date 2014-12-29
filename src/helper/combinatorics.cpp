#include <hop_bits/helper/combinatorics.hpp>

// C++ Standard Library
#include <cstdlib>

namespace hop {
  std::vector<arma::Col<unsigned int>> getCombinationsWithoutRepetition(const unsigned int& numberOfElements, const unsigned int& combinationSize) {
    std::vector<arma::Col<unsigned int>> combinations;

    arma::Col<unsigned int> combination = arma::linspace<arma::Col<unsigned int>>(0, combinationSize - 1, combinationSize);
    combinations.push_back(combination);

    while(true) {
      if (combination.at(combinationSize - 1) < numberOfElements - 1) {
        combination.at(combinationSize - 1)++;
        combinations.push_back(combination);
        continue;
      }

      std::size_t n;
      for (n = combinationSize - 1; n > 0; --n) {
        if (combination.at(n - 1) < numberOfElements - combinationSize + n - 1) {
          break;
        }
      }

      if (n < 1) {
        break;
      }

      combination.at(n - 1)++;
      for (;n < combinationSize; ++n) {
        combination.at(n) = combination.at(n - 1) + 1;
      }
      combinations.push_back(combination);
    }

    return combinations;
  }
}
