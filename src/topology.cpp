#include "mantella_bits/topology.hpp"

// Mantella
#include "mantella_bits/assert.hpp"

namespace mant {
  double mahalanobisDistance(
      const arma::Col<double>& firstParameter,
      const arma::Col<double>& secondParameter,
      const arma::Mat<double>& covariance) {
    verify(firstParameter.n_elem == secondParameter.n_elem, "mahalanobisDistance: The number of dimensions of the first parameter must be equal to the second one.");
    verify(firstParameter.n_elem == covariance.n_rows, "mahalanobisDistance: The number of rows of the covariance matrix must be equal to the number of dimensions of each parameter.");
    verify(isPositiveSemiDefinite(covariance), "mahalanobisDistance: The covariance matrix must be positive semi-definite.");

    const arma::Col<double>& difference = secondParameter - firstParameter;
    return std::sqrt(arma::dot(difference, arma::solve(covariance, difference)));
  }
}
