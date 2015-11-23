#pragma once

// Armadillo
#include <armadillo>

namespace mant {
  arma::Col<arma::uword> range(
      const arma::uword start,
      const arma::uword end,
      const arma::uword stepSize);

  arma::Col<arma::uword> range(
      const arma::uword start,
      const arma::uword end);

  class Hash {
   public:
    arma::uword operator()(
        const arma::Col<double>& key) const;
  };

  class IsEqual {
   public:
    bool operator()(
        const arma::Col<double>& firstKey,
        const arma::Col<double>& secondKey) const;
  };
}
