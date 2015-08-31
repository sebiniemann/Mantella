#include <mantella_bits/distanceFunction.hpp>

namespace mant {
  DistanceFunction::DistanceFunction(
      const arma::uword numberOfDimensions) 
    : numberOfDimensions_(numberOfDimensions){
    
  }

  double DistanceFunction::getDistance(
      const arma::Col<double>& firstParameter,
      const arma::Col<double>& secondParameter) const {
    return getLengthImplementation(secondParameter - firstParameter);
  }
  
  double DistanceFunction::getLength(
      const arma::Col<double>& parameter) const {
    return getLengthImplementation(parameter);
  }
}