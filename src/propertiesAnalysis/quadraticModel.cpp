#include <hop_bits/propertiesAnalysis/quadraticModel.hpp>

namespace hop {
  arma::Col<double> QuadraticModel::getQuadraticEstimator() {
    return quadraticEstimator_;
  }
}
