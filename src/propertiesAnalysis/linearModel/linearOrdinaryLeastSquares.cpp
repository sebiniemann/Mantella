#include <hop_bits/propertiesAnalysis/linearModel/linearOrdinaryLeastSquares.hpp>

// HOP
#include <hop_bits/helper/regression.hpp>

namespace hop {
  void LinearOrdinaryLeastSquares::analyseImplementation(
      const arma::Mat<double>& parameters,
      const arma::Col<double>& objectiveValues) {
    linearEstimator_ = Regression::getEstimator(parameters, objectiveValues);
  }
}
