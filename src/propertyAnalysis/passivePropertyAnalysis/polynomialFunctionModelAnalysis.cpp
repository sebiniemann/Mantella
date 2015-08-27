#include <mantella_bits/propertyAnalysis/passivePropertyAnalysis/polynomialFunctionModelAnalysis.hpp>

// C++ standard library
#include <cassert>

// Mantella
#include <mantella_bits/helper/regression.hpp>

namespace mant {
  PolynomialFunctionModelAnalysis::PolynomialFunctionModelAnalysis(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples,
      const std::shared_ptr<RegressionFunction> regressionFunction)
    : PassivePropertyAnalysis(samples),
      regressionFunction_(regressionFunction),
      residuals_(samples_.size()) {
    
  }

  void PolynomialFunctionModelAnalysis::analyseImplementation() {
    assert(samples_.size() > 1);
    
    arma::Mat<double> parameters(regressionFunction_->numberOfRegressionDimensions_, samples_.size());
    arma::Col<double> objectiveValues(samples_.size());
    arma::uword n = 0;
    for (const auto& sample : samples_) {
      parameters.col(n) = regressionFunction_->getRegression(sample.first);
      objectiveValues(n) = sample.second;
      ++n;
    }
    
    coefficients_ = getOrdinaryLeastSquaresEstimate(parameters, objectiveValues);
    
    for (n = 0; n < samples_.size(); ++n) {
      residuals_(n) = objectiveValues(n) - arma::dot(parameters.col(n), coefficients_);
    }
  }
      
  arma::Col<double> PolynomialFunctionModelAnalysis::getCoefficients() const {
    return coefficients_;
  }
  
  arma::Col<double> PolynomialFunctionModelAnalysis::getResiduals() const {
    return residuals_;
  }

  std::string PolynomialFunctionModelAnalysis::toString() const {
    return "polynomial_function_model_analysis";
  }
}
