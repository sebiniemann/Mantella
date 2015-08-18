#include <mantella_bits/surrogateModel.hpp>

// C++ standard library
#include <limits>

namespace mant {
  SurrogateModel::SurrogateModel(
      std::unordered_map<arma::Col<double>, double, Hash, IsEqual> samples) {
      
  }
  
  bool SurrogateModel::isFirstOrderDifferentiable() const {
    return false;
  }
  
  bool SurrogateModel::isSecondOrderDifferentiable() const {
    return false;
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
  
  double SurrogateModel::getFirstOrderDerivative(
      const arma::Col<double>& parameter) const {
    return getFirstOrderDerivativeImplementation(parameter);
  }
  
  double SurrogateModel::getSecondOrderDerivative(
      const arma::Col<double>& parameter) const {
    return getSecondOrderDerivativeImplementation(parameter);
  }
  
  double SurrogateModel::getFirstOrderDerivativeImplementation(
      const arma::Col<double>& parameter) const {
    return std::numeric_limits<double>::quiet_NaN();
  }
  
  double SurrogateModel::getSecondOrderDerivativeImplementation(
      const arma::Col<double>& parameter) const {
    return std::numeric_limits<double>::quiet_NaN();
  }
  
  double SurrogateModel::getObjectiveValue(
      const arma::Col<double>& parameter) const {
    return getObjectiveValueImplementation(parameter);
  }
}