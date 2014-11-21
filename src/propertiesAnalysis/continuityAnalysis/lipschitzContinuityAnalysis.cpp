#include <hop_bits/propertiesAnalysis/continuityAnalysis/lipschitzContinuityAnalysis.hpp>

namespace hop {
  double LipschitzContinuityAnalysis::getLipschitzConstant() const noexcept {
    return lipschitzConstant_;
  }
}
