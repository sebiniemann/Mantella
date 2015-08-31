#include <mantella_bits/optimisationProblem/surrogateModel/krigingModel.hpp>

// Mantella
#include <mantella_bits/helper/regression.hpp>

namespace mant {
  KrigingModel::KrigingModel(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples,
      const std::shared_ptr<RegressionFunction> regressionFunction,
      const std::shared_ptr<CorrelationFunction> correlationFunction)
    : SurrogateModel(samples),
      regressionFunction_(regressionFunction),
      correlationFunction_(correlationFunction) {
      
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
    
    for (n = 0; n < parameters.n_cols; ++n) {
      const arma::Col<double>& parameter = parameters.col(n);
      for (arma::uword k = n + 1; k < parameters.n_cols; ++k) {
        correlations(n, k) = correlationFunction_->getCorrelationCoefficient(parameters.col(k) - parameter);
      }
      
      parameters.col(n) = regressionFunction_->getRegression(parameter);
    }
    correlations = arma::symmatu(correlations);

    beta_ = getGeneralisedLeastSquaresEstimate(parameters, objectiveValues, correlations);
    try {
      gamma_ = correlations.i() * (objectiveValues - parameters * beta_);
    } catch (...) {
      gamma_ = arma::pinv(correlations) * (objectiveValues - parameters * beta_);
    }
  }
  
  double KrigingModel::getObjectiveValueImplementation(
      const arma::Col<double>& parameter) const {
    const arma::Col<double>& normalisedParameter = (parameter - meanParameter_) / standardDeviationParameter_;
    
    arma::Col<double> correlations(samples_.size());
    arma::uword n = 0;
    for (const auto& sample : samples_) {
      correlations(n++) = correlationFunction_->getCorrelationCoefficient(sample.first - normalisedParameter);
    }
    
    return meanObjectiveValue_ + (arma::dot(beta_, regressionFunction_->getRegression(parameter)) + arma::dot(gamma_, correlations)) * standardDeviationObjectiveValue_;
  }
  
  std::string KrigingModel::toString() const {
    return "kriging_model";
  }
}
