#include <mantella_bits/correlationFunction.hpp>

namespace mant {
  CorrelationFunction::CorrelationFunction(
      const arma::uword numberOfDimensions) 
    : numberOfDimensions_(numberOfDimensions) {
    setCorrelationWeights(arma::ones<arma::Col<double>>(numberOfDimensions_));
  }
  
  void CorrelationFunction::setCorrelationWeights(
      const arma::Col<double>& correlationWeights) {
    correlationWeights_ = correlationWeights;
  }
  
  arma::Col<double> CorrelationFunction::getCorrelationWeight() const {
    return correlationWeights_;
  }
  
  double CorrelationFunction::getCorrelationCoefficient(
      const arma::Col<double>& parameter) const {
    return getCorrelationCoefficientImplementation(correlationWeights_ % parameter);
  }
}
