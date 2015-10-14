#include <mantella_bits/helper/statistic.hpp>

// C++ standard library
#include <cmath>

namespace mant {
  double getMedianAbsoluteError(
      const arma::Row<double>& data) {
    return getMedian(arma::abs(data - getMedian(data)));
  }

  double getPercentile(
      const arma::Row<double>& data,
      const double nthPercentile) {
    double index = nthPercentile * data.n_elem / 100;

    arma::uword lowerIndex = static_cast<arma::uword>(std::floor(index));
    arma::uword upperIndex = static_cast<arma::uword>(std::ceil(index));

    if (lowerIndex != upperIndex) {
      return (index - lowerIndex) * data(lowerIndex) + (upperIndex - index) * data(upperIndex);
    } else {
      return data(upperIndex);
    }
  }

  double getDecile(
      const arma::Row<double>& data,
      const double nthDecile) {
    return getPercentile(data, nthDecile * 10.0);
  }

  double getQuartile(
      const arma::Row<double>& data,
      const double nthQuartile) {
    return getPercentile(data, nthQuartile * 25.0);
  }

  double getMedian(
      const arma::Row<double>& data) {
    return arma::median(data);
  }
}
