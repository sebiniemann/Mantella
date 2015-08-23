#pragma once

// Armadillo
#include <armadillo>

namespace mant {
  arma::Col<double> getOrdinaryLeastSquaresEstimate(
      const arma::Mat<double>& parameters,
      const arma::Col<double>& objectiveValues);
  
}
