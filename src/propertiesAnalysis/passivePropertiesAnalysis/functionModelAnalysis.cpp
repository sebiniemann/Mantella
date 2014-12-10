#include <hop_bits/propertiesAnalysis/passivePropertiesAnalysis/functionModelAnalysis.hpp>

namespace hop {
  arma::Col<double> FunctionModelAnalysis::getResiduals() const noexcept {
    return residuals_;
  }
}
