#include <hop_bits/propertiesAnalysis/passivePropertiesAnalysis/correlationAnalysis.hpp>

namespace hop {
  CorrelationAnalysis::CorrelationAnalysis() noexcept
    : correlationCoefficient_(0.0) {

  }

  double CorrelationAnalysis::getCorrelationCoefficient() const noexcept {
    return correlationCoefficient_;
  }
}
