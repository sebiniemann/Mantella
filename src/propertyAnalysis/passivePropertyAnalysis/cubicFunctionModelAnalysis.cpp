#include <mantella_bits/propertyAnalysis/passivePropertyAnalysis/cubicFunctionModelAnalysis.hpp>

// C++ standard library
#include <cassert>

namespace mant {
  CubicFunctionModelAnalysis::CubicFunctionModelAnalysis(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples) 
    : PassivePropertyAnalysis(samples),
      cubicCoefficients_(numberOfDimensions_, numberOfDimensions_, numberOfDimensions_),
      quadraticCoefficients_(numberOfDimensions_, numberOfDimensions_),
      errorTerm_(0),
      residuals_(samples_.size()) {
    
  }

  void CubicFunctionModelAnalysis::analyseImplementation() {
    assert(samples_.size() > 1);
    
    arma::Mat<double> parameters(numberOfDimensions_ * (numberOfDimensions_ + 1) * (numberOfDimensions_ + 2) / 6 + numberOfDimensions_ * (numberOfDimensions_ + 1) / 2 + numberOfDimensions_ + 1, samples_.size());
    arma::Col<double> objectiveValues(parameters.n_cols);
    arma::uword n = 0;
    for (const auto& sample : samples_) {
      const arma::Col<double>& parameter = sample.first;

      parameters.submat(0, n, numberOfDimensions_ - 1, n) = parameter;

      arma::uword k = 0;
      for (arma::uword l = 0; l < numberOfDimensions_; ++l) {
        for (arma::uword m = l; m < numberOfDimensions_; ++m) {
          for (arma::uword o = m; o < numberOfDimensions_; ++o) {
            parameters(k++, n) = parameter(l) * parameter(m) * parameter(o);
          }
        }
      }
      
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
        for (arma::uword m = l; m < numberOfDimensions_; ++m) {
          if (k != l && k != m && l != m) {
            const double& coefficient = model(n++) / 6.0;
            cubicCoefficients_(k, l, m) = coefficient;
            cubicCoefficients_(k, m, l) = coefficient;
            cubicCoefficients_(l, k, m) = coefficient;
            cubicCoefficients_(l, m, k) = coefficient;
            cubicCoefficients_(m, k, l) = coefficient;
            cubicCoefficients_(m, l, k) = coefficient;
          } else if (k != l) {
            const double& coefficient = model(n++) / 3.0;
            cubicCoefficients_(k, k, l) = coefficient;
            cubicCoefficients_(k, l, k) = coefficient;
            cubicCoefficients_(l, k, k) = coefficient;
          } else if (k != m) {
            const double& coefficient = model(n++) / 3.0;
            cubicCoefficients_(k, k, m) = coefficient;
            cubicCoefficients_(k, m, k) = coefficient;
            cubicCoefficients_(m, k, k) = coefficient;
          } else if (l != m) {
            const double& coefficient = model(n++) / 3.0;
            cubicCoefficients_(l, l, m) = coefficient;
            cubicCoefficients_(l, m, l) = coefficient;
            cubicCoefficients_(m, l, l) = coefficient;
          } else {
            cubicCoefficients_(k, l, m) = model(n++);
          }
        }
      }
    }
    
    for (arma::uword k = 0; k < numberOfDimensions_; ++k) {
      for (arma::uword l = k + 1; l < numberOfDimensions_; ++l) {
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
      
  arma::Cube<double> CubicFunctionModelAnalysis::getCubicCoefficients() const {
    return cubicCoefficients_;
  }
      
  arma::Mat<double> CubicFunctionModelAnalysis::getQuadraticCoefficients() const {
    return quadraticCoefficients_;
  }
      
  arma::Col<double> CubicFunctionModelAnalysis::getLinearCoefficients() const {
    return linearCoefficients_;
  }
      
  double CubicFunctionModelAnalysis::getErrorTerm() const {
    return errorTerm_;
  }
  
  arma::Col<double> CubicFunctionModelAnalysis::getResiduals() const {
    return residuals_;
  }

  std::string CubicFunctionModelAnalysis::toString() const {
    return "cubic_function_model_analysis";
  }
}
