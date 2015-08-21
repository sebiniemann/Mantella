#include <mantella_bits/surrogateModel/multivariateAdaptiveRegressionSplinesModel.hpp>

namespace mant {
  MultivariateAdaptiveRegressionSplinesModel::MultivariateAdaptiveRegressionSplinesModel(
      const arma::uword numberOfDimensions)
    : SurrogateModel(numberOfDimensions, false, false) {
    
  }

  void MultivariateAdaptiveRegressionSplinesModel::modelImplementation() {
  
  }
  
  double MultivariateAdaptiveRegressionSplinesModel::getObjectiveValueImplementation(
      const arma::Col<double>& parameter) const {
      return 0.0;
  }
  
  std::string MultivariateAdaptiveRegressionSplinesModel::toString() const {
    return "multivariate_adaptive_regression_splines_model";
  }
}