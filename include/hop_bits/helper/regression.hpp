#pragma once

// C++ Standard Library
#include <unordered_map>

// Armadillo
#include <armadillo>

// HOP
#include <hop_bits/helper/cache.hpp>

namespace hop {
  arma::Col<double> getEstimator(
      const arma::Mat<double>& regressor,
      const arma::Col<double>& regressand);
}
