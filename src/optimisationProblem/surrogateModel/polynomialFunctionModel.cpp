#include "mantella_bits/optimisationProblem/surrogateModel/polynomialFunctionModel.hpp"

// Mantella
#include "mantella_bits/propertyAnalysis/passivePropertyAnalysis/polynomialFunctionModelAnalysis.hpp"
#include "mantella_bits/regressionFunction.hpp"

namespace mant {
  PolynomialFunctionModel::PolynomialFunctionModel(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples,
      const std::shared_ptr<RegressionFunction> regressionFunction)
      : SurrogateModel(samples),
        regressionFunction_(regressionFunction) {
  }

  void PolynomialFunctionModel::modelImplementation() {
    PolynomialFunctionModelAnalysis polynomialFunctionAnalysis(samples_, regressionFunction_);
    polynomialFunctionAnalysis.analyse();

    coefficients_ = polynomialFunctionAnalysis.getCoefficients();
  }

  double PolynomialFunctionModel::getObjectiveValueImplementation(
      const arma::Col<double>& parameter) const {
    return arma::dot(coefficients_, regressionFunction_->getRegression(parameter));
  }

  std::string PolynomialFunctionModel::toString() const {
    return "polynomial_function_model";
  }
}
