#include "mantella_bits/kriging.hpp"

// C++ standard library
#include <cassert>

namespace mant {
  arma::Col<double> leastSquaresEstimate(
      const arma::mat& parameters,
      const arma::vec& objectiveValues) {
    assert(parameters.n_cols == objectiveValues.n_elem && "");

    return arma::solve(parameters * parameters.t(), parameters) * objectiveValues.t();
  }

  arma::Col<double> generalisedLeastSquaresEstimate(
      const arma::mat& parameters,
      const arma::vec& objectiveValues,
      const arma::mat& variance) {
    assert(parameters.n_cols == objectiveValues.n_elem && "");
    assert(parameters.n_cols == variance.n_rows && "");
    assert(variance.is_square() && "");

    arma::mat cholesky = arma::chol(variance);
    return leastSquaresEstimate(parameters * cholesky, objectiveValues * cholesky);
  }
}
