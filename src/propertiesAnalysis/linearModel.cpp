#include <hop_bits/propertiesAnalysis/linearModel.hpp>

namespace hop {
  arma::Col<double> LinearModel::getLinearEstimator() const {
    return linearEstimator_;
  }
}
