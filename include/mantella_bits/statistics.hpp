#pragma once

// C++ standard library
#include <string>

// Armadillo
#include <armadillo>

namespace mant {
  arma::Col<double> leastSquaresEstimate(
      const arma::mat& parameters,
      const arma::vec& objectiveValues);

  arma::Col<double> generalisedLeastSquaresEstimate(
      const arma::mat& parameters,
      const arma::vec& objectiveValues,
      const arma::mat& variance);
}
