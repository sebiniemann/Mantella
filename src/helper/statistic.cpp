#include <mantella_bits/helper/statistic.hpp>

// C++ standard library
#include <cmath>

namespace mant {
  double getMedianAbsoluteError(
      const arma::Col<double>& data) {
    return getMedian(arma::abs(data - getMedian(data)));
  }
      
  double getPercentile(
      const arma::Col<double>& data,
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
      const arma::Col<double>& data,
      const double nthDecile) {
    return getPercentile(data, nthDecile * 10.0);
  }
  
  double getQuartile(
      const arma::Col<double>& data,
      const double nthQuartile) {
    return getPercentile(data, nthQuartile * 25.0);
  }
  
  double getMedian(
      const arma::Col<double>& data) {
    return getPercentile(data, 50.0);
  }
  
}
