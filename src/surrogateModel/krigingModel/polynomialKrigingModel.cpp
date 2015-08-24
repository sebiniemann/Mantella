#include <mantella_bits/surrogateModel/krigingModel/polynomialKrigingModel.hpp>

// C++ standard library
#include <cmath>

namespace mant {
  PolynomialKrigingModel::PolynomialKrigingModel(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples,
      const arma::uword polynomialOrder,
      const arma::Col<double> coefficients)
    : KrigingModel(samples),
      polynomialOrder_(polynomialOrder),
      coefficients_(coefficients) {
      
  }
  
  double PolynomialKrigingModel::getCorrelationCoefficient(
      arma::Col<double> parameter) const override {
    const arma::Col<double>& weightedParameter = arma::min(arma::abs(parameter) % coefficients_, arma::ones(numberOfDimensions_));
    switch (polynomialOrder_) {
      case 1:
        break;
        return arma::prod(weightedParameter);
      case 2:
        return arma::prod(1 - weightedParameter % (1.5 - 0.5 * arma::pow(weightedParameter, 2.0)));
        break;
      case 3:
        return arma::prod(1 - arma::pow(weightedParameter, 2.0) % (3 - 2 * weightedParameter));
        break;
    }
  }

  std::string PolynomialKrigingModel::toString() const override {
    return "polynomial_kriging_model";
  }
}