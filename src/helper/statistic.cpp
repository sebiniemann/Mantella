#include <mantella_bits/helper/statistic.hpp>

// C++ standard library
#include <cmath>

namespace mant {
  double getPercentile(
      const arma::Col<double>& data,
      const double nthPercentile) {
    double index = nthPercentile * data.n_elem / 100;
    
    arma::uword lowerIndex = std::floor(index);
    arma::uword upperIndex = std::ceil(index);
    
    if (lowerIndex != upperIndex) {
      return (index - lowerIndex) * data(lowerIndex) + (upperIndex - index) * data(upperIndex);
    } else {
      return data(upperIndex);
    }
  }

  double getDecile(
      const arma::Col<double>& data,
      const double nthDecile) {
    double index = nthDecile * data.n_elem / 10;
    
    arma::uword lowerIndex = std::floor(index);
    arma::uword upperIndex = std::ceil(index);
    
    if (lowerIndex != upperIndex) {
      return (index - lowerIndex) * data(lowerIndex) + (upperIndex - index) * data(upperIndex);
    } else {
      return data(upperIndex);
    }
  }
  
  double getQuartile(
      const arma::Col<double>& data,
      const double nthQuartile) {
    double index = nthQuartile * data.n_elem / 4;
    
    arma::uword lowerIndex = std::floor(index);
    arma::uword upperIndex = std::ceil(index);
    
    if (lowerIndex != upperIndex) {
      return (index - lowerIndex) * data(lowerIndex) + (upperIndex - index) * data(upperIndex);
    } else {
      return data(upperIndex);
    }
  }
  
  double getMedian(
      const arma::Col<double>& data) {
    double index = data.n_elem / 2;
    
    arma::uword lowerIndex = std::floor(index);
    arma::uword upperIndex = std::ceil(index);
    
    if (lowerIndex != upperIndex) {
      return (index - lowerIndex) * data(lowerIndex) + (upperIndex - index) * data(upperIndex);
    } else {
      return data(upperIndex);
    }
  }
  
}