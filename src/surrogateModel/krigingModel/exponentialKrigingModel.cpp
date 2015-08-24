#include <mantella_bits/surrogateModel/krigingModel/exponentialKrigingModel.hpp>

// C++ standard library
#include <cmath>

namespace mant {
  ExponentialKrigingModel::ExponentialKrigingModel(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples,
      const arma::Col<double> coefficients) 
    : KrigingModel(samples),
      coefficients_(coefficients) {
      
  }
  
  double ExponentialKrigingModel::getCorrelationCoefficient(
      arma::Col<double> parameter) const override {
    return arma::prod(std::exp(arma::accu(-arma::abs(parameter) % coefficients_)));
  }

  std::string ExponentialKrigingModel::toString() const override {
    return "exponential_kriging_model";
  }
}