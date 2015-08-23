#include <mantella_bits/propertyAnalysis/passivePropertyAnalysis/linearFunctionModelAnalysis.hpp>

// C++ standard library
#include <cassert>

// Mantella
#include <mantella_bits/helper/regression.hpp>

namespace mant {
  LinearFunctionModelAnalysis::LinearFunctionModelAnalysis(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples) 
    : PassivePropertyAnalysis(samples),
      errorTerm_(0),
      residuals_(samples_.size()) {
    
  }
  
  void LinearFunctionModelAnalysis::analyseImplementation() {
    assert(samples_.size() > 1);
    
    arma::Mat<double> parameters(numberOfDimensions_ + 1, samples_.size());
    arma::Col<double> objectiveValues(parameters.n_cols);

    arma::uword n = 0;
    for(const auto& sample : samples_) {
        parameters.submat(0, n, numberOfDimensions_ - 1, n) = sample.first;
        objectiveValues(n) = sample.second;
        ++n;
    }
    parameters.row(parameters.n_rows - 1).fill(1.0);

    const arma::Col<double>& model = getOrdinaryLeastSquaresEstimate(parameters, objectiveValues);
    
    coefficients_ = model.head(numberOfDimensions_);
    errorTerm_ = model(model.n_elem - 1);
    
    for (arma::uword n = 0; n < samples_.size(); ++n) {
      residuals_(n) = objectiveValues(n) - arma::dot(parameters.col(n), model);
    }
  }
  
  arma::Col<double> LinearFunctionModelAnalysis::getCoefficients() const {
    return coefficients_;
  }
  
  double LinearFunctionModelAnalysis::getErrorTerm() const {
    return errorTerm_;
  }
  
  arma::Col<double> LinearFunctionModelAnalysis::getResiduals() const {
    return residuals_;
  }

  std::string LinearFunctionModelAnalysis::toString() const {
    return "linear_function_model_analysis";
  }
}
