// Armadillo
#include <armadillo>

// C++ standard library
#include <cassert>

namespace mant {
  arma::Col<double> leastSquaresEstimate(
      const arma::mat& parameters,
      const arma::vec& objectiveValues) {
    assert(parameters.n_cols == objectiveValues.n_elem && "leastSquaresEstimate: the number of parameters must match the number of objective values.");

    return arma::solve(parameters * parameters.t(), parameters) * objectiveValues.t();
  }

  arma::Col<double> generalisedLeastSquaresEstimate(
      const arma::mat& parameters,
      const arma::vec& objectiveValues,
      const arma::mat& variance) {
    assert(parameters.n_cols == objectiveValues.n_elem && "generalisedLeastSquaresEstimate: the number of parameters must match the number of objective values.");
    assert(parameters.n_cols == variance.n_rows && "generalisedLeastSquaresEstimate: the number of parameters must match the number of variance values.");
    assert(variance.is_square() && "generalisedLeastSquaresEstimate: variance must be a square matrix.");

    arma::mat cholesky = arma::chol(variance);
    return leastSquaresEstimate(parameters * cholesky, objectiveValues * cholesky);
  }
}
