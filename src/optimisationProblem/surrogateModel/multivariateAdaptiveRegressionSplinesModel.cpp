#include "mantella_bits/optimisationProblem/surrogateModel/multivariateAdaptiveRegressionSplinesModel.hpp"

namespace mant {
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
