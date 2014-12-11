#include <hop_bits/propertiesAnalysis/passivePropertiesAnalysis/functionModelAnalysis/quadraticModelAnalysis.hpp>

namespace hop {
  QuadraticModelAnalysis::QuadraticModelAnalysis() noexcept {
    setQuadraticModelResidualsThreshold(0.75);
  }

  arma::Col<double> QuadraticModelAnalysis::getQuadraticModelEstimator() const noexcept {
    return quadraticModelEstimator_;
  }

  void QuadraticModelAnalysis::setQuadraticModelResidualsThreshold(
      const double& quadraticModelResidualsThreshold) {
    if(quadraticModelResidualsThreshold < 0 || quadraticModelResidualsThreshold > 1) {
      throw std::runtime_error("The quadratic model residuals threshold (" + std::to_string(quadraticModelResidualsThreshold) + ") must be within 0 and 1.");
    }

    quadraticModelResidualsThreshold_ = quadraticModelResidualsThreshold;
  }
}
