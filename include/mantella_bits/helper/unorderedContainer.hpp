#pragma once

// Armadillo
#include <armadillo>

namespace mant {
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
