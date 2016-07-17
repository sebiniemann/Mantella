#pragma once

// C++ standard library
#include <utility>
#include <vector>

// Armadillo
#include <armadillo>

namespace mant {
  arma::uword factorial(
      const arma::uword n);

  arma::uword nchoosek(
      arma::uword n,
      const arma::uword k);

  arma::uword secondStirlingNumber(
      const arma::uword numberOfElements,
      const arma::uword numberOfParts);

  std::vector<arma::uvec> combinations(
      const arma::uword numberOfElements,
      const arma::uword cycleSize);

  std::vector<arma::uvec> multicombinations(
      const arma::uword numberOfElements,
      const arma::uword cycleSize);

  std::vector<std::pair<arma::uvec, arma::uvec>> twoSetsPartitions(
      const arma::uword numberOfElements);
}
