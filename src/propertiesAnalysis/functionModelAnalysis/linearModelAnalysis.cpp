#include <hop_bits/propertiesAnalysis/functionModelAnalysis/linearModelAnalysis.hpp>

namespace hop {
  arma::Col<double> LinearModelAnalysis::getLinearModelEstimator() const noexcept {
    return linearModelEstimator_;
  }
}
