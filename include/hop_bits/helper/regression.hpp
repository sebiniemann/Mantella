#pragma once

// Armadillo
#include <armadillo>

namespace hop {
  class Regression {
    public:
      arma::Col<double> getEstimator(
          const arma::Mat<double>& regressors,
          const arma::Col<double>& regressand);
  };
}
