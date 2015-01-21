#pragma once

// C++ Standard Library
#include <cstdlib>
#include <vector>
#include <utility>

// Armadillo
#include <armadillo>

namespace mant {
  inline std::vector<std::pair<arma::Col<unsigned int>, arma::Col<unsigned int>>> getTwoSetsPartitions(
      const unsigned int& numberOfElements) ;

  //
  // Implementation
  //

  inline std::vector<std::pair<arma::Col<unsigned int>, arma::Col<unsigned int>>> getTwoSetsPartitions(
      const unsigned int& numberOfElements)  {
    arma::Col<unsigned int> elements = arma::linspace<arma::Col<unsigned int>>(0, numberOfElements - 1, numberOfElements);

    std::vector<std::pair<arma::Col<unsigned int>, arma::Col<unsigned int>>> partitions;

    for(int n = 1; n <= std::floor(numberOfElements / 2.0); ++n) {
      arma::Col<unsigned int> firstSet = elements.head(n);
      arma::Col<unsigned int> secondSet = elements.tail(elements.n_elem - n);

      partitions.push_back({firstSet, secondSet});

      arma::Col<unsigned int> secondSetIndex(n, arma::fill::zeros);
      int firstSetIndex = firstSet.size() - 1;

      int lastIndex = -1;
      if(firstSet.size() == secondSet.size()) {
        lastIndex = 0;
      }

      while (firstSetIndex != lastIndex) {
        std::iter_swap(firstSet.begin() + firstSetIndex, secondSet.begin() + secondSetIndex.at(firstSetIndex));
        partitions.push_back({firstSet, secondSet});

        if(secondSetIndex.at(firstSetIndex) >= secondSet.size() - 1) {
          --firstSetIndex;
        } else {
          secondSetIndex.tail(firstSet.size() - firstSetIndex) += 1;
          firstSetIndex = firstSet.size() - 1;
        }
      }
    }

    return partitions;
  }
}
