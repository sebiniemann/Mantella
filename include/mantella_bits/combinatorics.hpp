#pragma once

// C++ standard library
#include <vector>

// Armadillo
#include <armadillo>

namespace mant {
  arma::uword factorial(
      const arma::uword n);

  arma::uword nchoosek(
      arma::uword n,
      const arma::uword k);

  std::vector<arma::Col<arma::uword>> combinations(
      const arma::uword numberOfElements,
      const arma::uword combinationSize);

  std::vector<arma::Col<arma::uword>> multicombinations(
      const arma::uword numberOfElements,
      const arma::uword combinationsize);
}
