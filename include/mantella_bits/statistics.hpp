#pragma once

// Armadillo
#include <armadillo>

namespace mant {
  double medianAbsoluteError(
      const arma::Row<double>& data);

  double percentile(
      const arma::Row<double>& data,
      const double nthPercentile);

  double decile(
      const arma::Row<double>& data,
      const double nthDecile);

  double quartile(
      const arma::Row<double>& data,
      const double nthQuartile);

  arma::Col<double> ordinaryLeastSquaresEstimate(
      const arma::Mat<double>& parameters,
      const arma::Row<double>& objectiveValues);

  arma::Col<double> generalisedLeastSquaresEstimate(
      const arma::Mat<double>& parameters,
      const arma::Row<double>& objectiveValues,
      const arma::Mat<double>& covariance);
}
