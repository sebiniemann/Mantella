#include <hop_bits/helper/regression.hpp>

namespace hop {
  arma::Col<double> Regression::getEstimator(
      const arma::Mat<double>& regressors,
      const arma::Col<double>& regressand) {
    return (regressors.t() * regressors).i() * regressors.t() * regressand;
  }
}
