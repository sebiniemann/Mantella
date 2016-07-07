#pragma once

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/randomNumberGenerator.hpp"

namespace mant {
  class Hash {
   public:
    arma::uword operator()(
        const arma::vec& key) const;
  };

  class IsEqual {
   public:
    bool operator()(
        const arma::vec& firstKey,
        const arma::vec& secondKey) const;
  };

  arma::mat randu(
      const arma::uword numberOfRows,
      const arma::uword numberOfColumns);

  arma::vec randu(
      const arma::uword numberOfElements);

  arma::mat randn(
      const arma::uword numberOfRows,
      const arma::uword numberOfColumns);

  arma::vec randn(
      const arma::uword numberOfElements);
}
