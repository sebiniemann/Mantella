#include <mantella_bits/surrogateModel/krigingModel/splineKrigingModel.hpp>

namespace mant {
  SplineKrigingModel::SplineKrigingModel(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples,
      const arma::Col<double> coefficients)
    : KrigingModel(samples),
      coefficients_(coefficients) {
      
  }
  
  double SplineKrigingModel::getCorrelationCoefficient(
      arma::Col<double> parameter) const override {
    const arma::Col<double>& weightedParameter = arma::abs(parameter) % coefficients_;

    const arma::Col<arma::uword>& firstPart = arma::find(weightedParameter <= 0.2);
    const arma::Col<arma::uword>& secondPart = arma::find(0.2 < weightedParameter && weightedParameter < 1);

    weightedParameter(firstPart) = 1 - arma::pow(weightedParameter(firstPart), 2.0) % (15  - 30 * weightedParameter(firstPart));
    weightedParameter(secondPart) = 1.25 * (1.0 - arma::pow(weightedParameter(secondPart), 3.0));

    return arma::prod(weightedParameter);
  }

  std::string SplineKrigingModel::toString() const override {
    return "spline_kriging_model";
  }
}