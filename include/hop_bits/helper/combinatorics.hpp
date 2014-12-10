#pragma once

// C++ Standard Library
#include <vector>

// Armadillo
#include <armadillo>

namespace hop {
  std::vector<arma::Col<arma::uword>> getCombinationsWithoutRepetition(
      const unsigned int& numberOfElements,
      const unsigned int& combinationSize);
}
