#include <hop_bits/propertiesAnalysis/functionModelAnalysis/linearModelAnalysis.hpp>

namespace hop {
  arma::Col<double> LinearModelAnalysis::getLinearModelEstimator() const {
    return linearModelEstimator_;
  }
}
