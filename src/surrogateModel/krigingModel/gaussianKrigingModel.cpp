#include <mantella_bits/surrogateModel/krigingModel/gaussianKrigingModel.hpp>

namespace mant {
  GaussianKrigingModel::GaussianKrigingModel(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples,
      const arma::Col<double> coefficients) 
    : KrigingModel(samples),
      coefficients_(coefficients) {
      
  }
  
  double GaussianKrigingModel::getCorrelationCoefficient(
      arma::Col<double> parameter) const override {
    return arma::prod(std::exp(arma::accu(-arma::square(parameter) % coefficients_)));
  }

  std::string GaussianKrigingModel::toString() const override {
    return "gaussian_kriging_model";
  }
}