#include <mantella_bits/surrogateModel.hpp>

// C++ standard library
#include <limits>

namespace mant {
  SurrogateModel::SurrogateModel(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples) 
    : numberOfDimensions_ (samples.cbegin()->first.n_elem),
      samples_(samples) {
      
  }
  
  void SurrogateModel::setLowerBounds(
    const arma::Col<double>& lowerBounds) {
    lowerBounds_ = lowerBounds;
  }
  void SurrogateModel::setUpperBounds(
    const arma::Col<double>& upperBounds) {
    upperBounds_ = upperBounds;
  }

  arma::Col<double> SurrogateModel::getLowerBounds() const {
    return lowerBounds_;
  }
  arma::Col<double> SurrogateModel::getUpperBounds() const {
    return upperBounds_;
  }
  
  void SurrogateModel::model() {
    modelImplementation();
  }
  
  double SurrogateModel::getObjectiveValue(
      const arma::Col<double>& parameter) const {
    return getObjectiveValueImplementation(parameter);
  }
}