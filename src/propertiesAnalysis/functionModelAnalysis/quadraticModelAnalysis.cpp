#include <hop_bits/propertiesAnalysis/functionModelAnalysis/quadraticModelAnalysis.hpp>

namespace hop {
  arma::Col<double> QuadraticModelAnalysis::getQuadraticModelEstimator() const {
    return quadraticModelEstimator_;
  }
}
