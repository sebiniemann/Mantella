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
  
  arma::Col<double> SurrogateModel::getJacobian(
      const arma::Col<double>& parameter) const {
    return getJacobianImplementation(parameter);
  }
  
  arma::Mat<double> SurrogateModel::getHessian(
      const arma::Col<double>& parameter) const {
    return getHessianImplementation(parameter);
  }
  
  arma::Col<double> SurrogateModel::getJacobianImplementation(
      const arma::Col<double>& parameter) const {
    return arma::Col<double>(numberOfDimensions_, arma::fill::zeros);
  }
  
  arma::Mat<double> SurrogateModel::getHessianImplementation(
      const arma::Col<double>& parameter) const {
    return arma::Mat<double>(numberOfDimensions_, numberOfDimensions_, arma::fill::zeros);
  }
  
  double SurrogateModel::getObjectiveValue(
      const arma::Col<double>& parameter) const {
    return getObjectiveValueImplementation(parameter);
  }
}