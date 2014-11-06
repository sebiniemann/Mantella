#include <hop_bits/helper/regression.hpp>

// C++ Standard Library
#include <stdexcept>

namespace hop {
  arma::Col<double> Regression::getEstimator(
      const arma::Mat<double>& regressors,
      const arma::Col<double>& regressand) {
    // TODO Check size

    return (regressors.t() * regressors).i() * regressors.t() * regressand;
  }
}
