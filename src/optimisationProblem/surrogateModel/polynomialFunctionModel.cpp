#include "mantella_bits/optimisationProblem/surrogateModel/polynomialFunctionModel.hpp"

// Mantella
#include "mantella_bits/algebra.hpp"
#include "mantella_bits/samplesAnalysis.hpp"

namespace mant {
  PolynomialFunctionModel::PolynomialFunctionModel(
      const arma::uword numberOfDimensions)
      : SurrogateModel(numberOfDimensions) {
    setModelFunction([this](
        const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples) {
        coefficients_ = polynomialFunctionModelAnalysis(samples, polynomialOrder_, estimatorFunction_);
    
        return [this] (
                        const arma::Col<double>& parameter) {
          return arma::dot(coefficients_, polynomial(parameter, polynomialOrder_));
        };
    });
  }

  void PolynomialFunctionModel::setPolynomialOrder(
      const arma::uword polynomialOrder) {
    polynomialOrder_ = polynomialOrder;
  }

  void PolynomialFunctionModel::setEstimatorFunction(
      const std::function<arma::Col<double>(const arma::Mat<double>& parameters, const arma::Row<double>& objectiveValues)> estimatorFunction) {
    estimatorFunction_ = estimatorFunction;
  }
}
