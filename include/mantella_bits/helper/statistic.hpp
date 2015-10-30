#pragma once

// Armadillo
#include <armadillo>

namespace mant {
  double getMedianAbsoluteError(
      const arma::Row<double>& data);

  double getPercentile(
      const arma::Row<double>& data,
      const double nthPercentile);

  double getDecile(
      const arma::Row<double>& data,
      const double nthDecile);

  double getQuartile(
      const arma::Row<double>& data,
      const double nthQuartile);

  double getMedian(
      const arma::Row<double>& data);
}
