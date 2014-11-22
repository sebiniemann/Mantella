#include <hop_bits/helper/regression.hpp>

// C++ Standard Library
#include <stdexcept>
#include <iostream>
#include <stdexcept>

namespace hop {
  arma::Col<double> getOrdinaryLeastSquaresEstimator(
      const arma::Mat<double>& regressor,
      const arma::Col<double>& regressand) {
    if(regressor.n_cols != regressand.n_rows) {
      throw std::runtime_error("The number of observations within the regressor (" + std::to_string(regressor.n_cols) + ") and regressand (" + std::to_string(regressand.n_rows) + ") must match.");
    }

    return (regressor * regressor.t()).i() * regressor * regressand;
  }
}
