#include "mantella_bits/kriging.hpp"

// C++ standard library
#include <utility>

// Mantella
#include "mantella_bits/armadillo.hpp"
#include "mantella_bits/assert.hpp"

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

  Kriging::Kriging(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples,
      const std::function<arma::Col<double>(const arma::Col<double>&)> regressionFunction,
      const std::function<double(const arma::Col<double>&)> correlationFunction)
      : regressionFunction_(regressionFunction),
        correlationFunction_(correlationFunction) {
  }

  void Kriging::train() {
    arma::Mat<double> parameters(samples_.begin()->first.n_elem, samples_.size());
    arma::Row<double> objectiveValues(samples_.size());
    arma::uword n = 0;
    for (const auto& sample : samples_) {
      parameters.col(n) = sample.first;
      objectiveValues(n) = sample.second;
      ++n;
    }

    meanParameter_ = arma::mean(parameters, 2);
    standardDeviationParameter_ = arma::stddev(parameters, 2);

    parameters.each_col() -= meanParameter_;
    parameters.each_col() /= standardDeviationParameter_;

    meanObjectiveValue_ = arma::mean(objectiveValues);
    standardDeviationObjectiveValue_ = arma::stddev(objectiveValues);

    objectiveValues -= meanObjectiveValue_;
    objectiveValues /= standardDeviationObjectiveValue_;

    arma::Mat<double> correlations(parameters.n_cols, parameters.n_cols);
    correlations.diag().zeros();

    for (n = 0; n < parameters.n_cols; ++n) {
      const arma::Col<double>& parameter = parameters.col(n);
      for (arma::uword k = n + 1; k < parameters.n_cols; ++k) {
        correlations(n, k) = correlationFunction_(parameters.col(k) - parameter);
      }

      parameters.col(n) = regressionFunction_(parameter);
    }
    correlations = arma::symmatu(correlations);

    beta_ = getGeneralisedLeastSquaresEstimate(parameters, objectiveValues, correlations);
    gamma_ = arma::solve(correlations, objectiveValues.t() - parameters * beta_);
  }

  double Kriging::predict(
      const arma::Col<double>& parameter) const {
    const arma::Col<double>& normalisedParameter = (parameter - meanParameter_) / standardDeviationParameter_;

    arma::Col<double> correlations(samples_.size());
    arma::uword n = 0;
    for (const auto& sample : samples_) {
      correlations(n++) = correlationFunction_(sample.first - normalisedParameter);
    }

    return meanObjectiveValue_ + (arma::dot(beta_, regressionFunction_(parameter)) + arma::dot(gamma_, correlations)) * standardDeviationObjectiveValue_;
  }
}
