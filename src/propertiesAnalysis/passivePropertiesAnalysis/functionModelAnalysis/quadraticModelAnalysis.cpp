#include <hop_bits/propertiesAnalysis/passivePropertiesAnalysis/functionModelAnalysis/quadraticModelAnalysis.hpp>

namespace hop {
  arma::Col<double> QuadraticModelAnalysis::getQuadraticModelEstimator() const noexcept {
    return quadraticModelEstimator_;
  }
}
