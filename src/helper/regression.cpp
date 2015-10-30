#include <mantella_bits/helper/regression.hpp>

// Mantella
#include <mantella_bits/helper/assert.hpp>

namespace mant {
  arma::Col<double> getOrdinaryLeastSquaresEstimate(
      const arma::Mat<double>& parameters,
      const arma::Row<double>& objectiveValues) {
    verify(parameters.n_cols == objectiveValues.n_elem, "");
      
    return arma::solve(parameters * parameters.t(), parameters) * objectiveValues.t();
  }

  arma::Col<double> getGeneralisedLeastSquaresEstimate(
      const arma::Mat<double>& parameters,
      const arma::Row<double>& objectiveValues,
      const arma::Mat<double>& variance) {
    verify(parameters.n_cols == objectiveValues.n_elem, "");
    verify(parameters.n_cols == variance.n_rows, "");
    verify(variance.is_square(), "");
    
    arma::Mat<double> cholesky = arma::chol(variance);
    return getOrdinaryLeastSquaresEstimate(parameters * cholesky, objectiveValues * cholesky);
  }
}
