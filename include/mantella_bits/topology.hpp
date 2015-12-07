#pragma once

// Armadillo
#include <armadillo>

namespace mant {
  double mahalanobisDistance(
      const arma::Col<double>& firstParameter,
      const arma::Col<double>& secondParameter,
      const arma::Mat<double>& covariance);
}
