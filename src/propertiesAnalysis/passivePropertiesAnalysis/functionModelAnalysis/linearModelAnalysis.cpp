#include <hop_bits/propertiesAnalysis/passivePropertiesAnalysis/functionModelAnalysis/linearModelAnalysis.hpp>

namespace hop {
  arma::Col<double> LinearModelAnalysis::getLinearModelEstimator() const noexcept {
    return linearModelEstimator_;
  }

  void LinearModelAnalysis::setLinearModelResidualsThreshold(
      const double& linearModelResidualsThreshold) {
    if(linearModelResidualsThreshold < 0 || linearModelResidualsThreshold > 1) {
      throw std::runtime_error("The quadratic model residuals threshold (" + std::to_string(linearModelResidualsThreshold) + ") must be within 0 and 1.");
    }

    linearModelResidualsThreshold_ = linearModelResidualsThreshold;
  }
}
