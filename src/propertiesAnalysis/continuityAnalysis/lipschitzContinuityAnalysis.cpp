#include <hop_bits/propertiesAnalysis/continuityAnalysis/lipschitzContinuityAnalysis.hpp>

namespace hop {
  LipschitzContinuityAnalysis::LipschitzContinuityAnalysis()
    : lipschitzConstant_(0) {

  }

  double LipschitzContinuityAnalysis::getLipschitzConstant() const noexcept {
    return lipschitzConstant_;
  }
}
