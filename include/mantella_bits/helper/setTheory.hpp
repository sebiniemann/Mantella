#pragma once

// C++ standard library
#include <vector>
#include <utility>

// Armadillo
#include <armadillo>

namespace mant {
  std::vector<std::pair<arma::Col<arma::uword>, arma::Col<arma::uword>>> getTwoSetsPartitions(
      const arma::uword numberOfElements);
}
