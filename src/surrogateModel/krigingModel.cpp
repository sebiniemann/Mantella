#include <mantella_bits/surrogateModel/krigingModel.hpp>

// Mantella
#include <mantella_bits/helper/regression.hpp>

namespace mant {
  KrigingModel::KrigingModel(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples,
      const arma::uword polynomialOrder)
    : SurrogateModel(samples),
      polynomialOrder_(polynomialOrder) {
      
  }
  
  void KrigingModel::modelImplementation() {
    arma::Mat<double> parameters(numberOfDimensions_, samples_.size());
    arma::Col<double> objectiveValues(samples_.size());
    arma::uword n = 0;
    for (const auto& sample : samples_) {
      parameters.col(n) = sample.first;
      objectiveValues(n) = sample.second;
      ++n;
    }
    
    meanParameter_ = arma::mean(parameters, 2);
    standardDeviationParameter_ = arma::stddev(parameters, 2);
    
    parameters.each_col() -= meanParameter_;
    parameters.each_col() /= standardDeviationParameter_;
    
    meanObjectiveValue_ = arma::mean(objectiveValues);
    standardDeviationObjectiveValue_ = arma::stddev(objectiveValues);
    
    objectiveValues -= meanObjectiveValue_;
    objectiveValues /= standardDeviationObjectiveValue_;
    
    arma::Mat<double> correlations(parameters.n_cols, parameters.n_cols);
    correlations.diag().zeros();
    
    for (arma::uword n = 0; n < parameters.n_cols; ++n) {
      const arma::Col<double>& parameter = parameters.col(n);
      for (arma::uword k = n + 1; k < parameters.n_cols; ++k) {
        correlations(n, k) = getCorrelationCoefficient(parameters.col(k) - parameter);
       ;
      }
      
      parameters.col(n) = getNthOrderRegression(parameter);
    }
    correlations = arma::symmatu(correlations);

    beta_ = getGeneralisedLeastSquaresEstimate(parameters, objectiveValues, correlations);
    try {
      gamma_ = correlations.i() * (objectiveValues - parameters * beta_);
    } catch (...) {
      gamma_ = arma::pinv(correlations) * (objectiveValues - parameters * beta_);
    }
  }
      
  arma::Col<double> KrigingModel::getNthOrderRegression(
      const arma::Col<double>& parameter) const {
    arma::Col<double> regression;
    
    switch (polynomialOrder_) {
      case 0:
        regression.set_size(numberOfDimensions_);
        regression.ones();
        break;
      case 1:
        regression.set_size(2 * numberOfDimensions_);
        regression.head(numberOfDimensions_).ones();
        regression.tail(numberOfDimensions_) = parameter;
        break;
      case 2:
        regression.set_size(numberOfDimensions_ * (numberOfDimensions_ + 1) / 2 + numberOfDimensions_ + 1);
        
        regression.head(numberOfDimensions_).ones();
        regression.subvec(numberOfDimensions_, 2 * numberOfDimensions_ - 1) = parameter;
        
        arma::uword n = 2 * numberOfDimensions_;
        for (arma::uword k = 0; k < numberOfDimensions_; ++k) {
          for (arma::uword l = k; l < numberOfDimensions_; ++l) {
            regression(n++) = parameter(k) * parameter(l);
          }
        }
        break;
    }
    
    return regression;
  }
  
  double KrigingModel::getObjectiveValueImplementation(
      const arma::Col<double>& parameter) const {
    const arma::Col<double>& normalisedParameter = (parameter - meanParameter_) / standardDeviationParameter_;
    
    arma::Col<double> correlations(samples_.size());
    arma::uword n = 0;
    for (const auto& sample : samples_) {
      correlations(n++) = getCorrelationCoefficient(sample.first - normalisedParameter);
    }
    
    return meanObjectiveValue_ + (arma::dot(beta_, getNthOrderRegression(parameter)) + arma::dot(gamma_, correlations)) * standardDeviationObjectiveValue_;
  }
}