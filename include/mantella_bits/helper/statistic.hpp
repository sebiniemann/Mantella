#pragma once

// Armadillo
#include <armadillo>

namespace mant {
  double getMedianAbsoluteError(
      const arma::Col<double>& data);
      
  double getPercentile(
      const arma::Col<double>& data,
      const double nthPercentile);

  double getDecile(
      const arma::Col<double>& data,
      const double nthDecile);
  
  double getQuartile(
      const arma::Col<double>& data,
      const double nthQuartile);
  
  double getMedian(
      const arma::Col<double>& data);
  
}
