#pragma once

// C++ standard library
#include <vector>
#include <utility>

// Armadillo
#include <armadillo>

namespace mant {
  std::vector<arma::Col<arma::uword>> getCombinations(
      const arma::uword numberOfElements,
      const arma::uword combinationSize);

  std::vector<std::pair<arma::Col<arma::uword>, arma::Col<arma::uword>>> getTwoSetsPartitions(
      const arma::uword numberOfElements);
}
