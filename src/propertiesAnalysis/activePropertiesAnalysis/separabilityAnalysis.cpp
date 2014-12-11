#include <hop_bits/propertiesAnalysis/activePropertiesAnalysis/separabilityAnalysis.hpp>

namespace hop {
  void SeparabilityAnalysis::setMaximalNumberOfIterations(
      const unsigned int& maximalNumberOfIterations) noexcept {
    maximalNumberOfIterations_ = maximalNumberOfIterations;
  }
}
