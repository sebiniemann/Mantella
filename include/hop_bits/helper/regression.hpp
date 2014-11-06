#pragma once

// C++ Standard Library
#include <unordered_map>

// Armadillo
#include <armadillo>

// HOP
#include <hop_bits/helper/cache.hpp>

namespace hop {
  class Regression {
    public:
      static arma::Col<double> getEstimator(
          const arma::Mat<double>& regressors,
          const arma::Col<double>& regressand);
  };
}
