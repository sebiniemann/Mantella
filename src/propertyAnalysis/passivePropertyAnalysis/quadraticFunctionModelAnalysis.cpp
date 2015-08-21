#include <mantella_bits/propertyAnalysis/passivePropertyAnalysis/quadraticFunctionModelAnalysis.hpp>

// C++ standard library
#include <cassert>

namespace mant {
  QuadraticFunctionModelAnalysis::QuadraticFunctionModelAnalysis(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples) 
    : PassivePropertyAnalysis(samples),
      quadraticCoefficients_(numberOfDimensions_, numberOfDimensions_),
      errorTerm_(0),
      residuals_(samples_.size()) {
    
  }

  void QuadraticFunctionModelAnalysis::analyseImplementation() {
    assert(samples_.size() > 1);
    
    arma::Mat<double> parameters(numberOfDimensions_ * (numberOfDimensions_ + 1) / 2 + numberOfDimensions_ + 1, samples_.size());
    arma::Col<double> objectiveValues(parameters.n_cols);
    arma::uword n = 0;
    for (const auto& sample : samples_) {
      const arma::Col<double>& parameter = sample.first;

      arma::uword k = 0;
      for (arma::uword l = 0; l < numberOfDimensions_; ++l) {
        for (arma::uword m = l; m < numberOfDimensions_; ++m) {
          parameters(k++, n) = parameter(l) * parameter(m);
        }
      }
      
      parameters.submat(k, n, k + numberOfDimensions_ - 1, n) = parameter;
      parameters(k + numberOfDimensions_,  n) = 1;

      objectiveValues(n) = sample.second;
      ++n;
    }
    
    arma::Col<double> model;
    try {
      model = (parameters * parameters.t()).i() * parameters * objectiveValues;
    } catch (...) {
      model = arma::pinv(parameters * parameters.t()) * parameters * objectiveValues;
    }
    
    n = 0;
    for (arma::uword k = 0; k < numberOfDimensions_; ++k) {
      for (arma::uword l = k; l < numberOfDimensions_; ++l) {
        if (k != l) {
          quadraticCoefficients_(k, l) = model(n++) / 2.0;
        } else {
          quadraticCoefficients_(k, l) = model(n++);
        }
      }
    }
    quadraticCoefficients_ = arma::symmatu(quadraticCoefficients_);
    
    linearCoefficients_ = model.subvec(n, n + numberOfDimensions_ - 1);
    errorTerm_ = model(n + numberOfDimensions_);
    
    for (arma::uword k = 0; k < samples_.size(); ++k) {
      residuals_(k) = objectiveValues(k) - arma::dot(parameters.col(k), model);
    }
  }
      
  arma::Mat<double> QuadraticFunctionModelAnalysis::getQuadraticCoefficients() const {
    return quadraticCoefficients_;
  }
      
  arma::Col<double> QuadraticFunctionModelAnalysis::getLinearCoefficients() const {
    return linearCoefficients_;
  }
      
  double QuadraticFunctionModelAnalysis::getErrorTerm() const {
    return errorTerm_;
  }
  
  arma::Col<double> QuadraticFunctionModelAnalysis::getResiduals() const {
    return residuals_;
  }

  std::string QuadraticFunctionModelAnalysis::toString() const {
    return "quadratic_function_model_analysis";
  }
}
