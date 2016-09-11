#pragma once

// Armadillo
#include <armadillo>

namespace mant {
  std::pair<arma::mat, arma::rowvec> elitists(
      const arma::mat& parameters,
      const arma::rowvec& objectiveValues,
      const arma::uword numberOfSamplesToSelect);
}
