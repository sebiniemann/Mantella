#include <hop_bits/propertiesAnalysis/passivePropertiesAnalysis/functionModelAnalysis/linearModelAnalysis.hpp>

namespace hop {
  arma::Col<double> LinearModelAnalysis::getLinearModelEstimator() const noexcept {
    return linearModelEstimator_;
  }
}
