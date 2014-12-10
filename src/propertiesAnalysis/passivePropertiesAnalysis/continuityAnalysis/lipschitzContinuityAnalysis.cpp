#include <hop_bits/propertiesAnalysis/passivePropertiesAnalysis/continuityAnalysis/lipschitzContinuityAnalysis.hpp>

namespace hop {
  LipschitzContinuityAnalysis::LipschitzContinuityAnalysis() noexcept
    : lipschitzConstant_(0.0) {

  }

  double LipschitzContinuityAnalysis::getLipschitzConstant() const noexcept {
    return lipschitzConstant_;
  }
}
