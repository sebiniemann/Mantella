#include <hop_bits/propertiesAnalysis/activePropertiesAnalysis/separabilityAnalysis/additiveSeparabilityAnalysis.hpp>

namespace hop {
  AdditiveSeparabilityAnalysis::AdditiveSeparabilityAnalysis() noexcept {
    setAdditiveSeparabilityThreshold(0.75);
  }

  void AdditiveSeparabilityAnalysis::setAdditiveSeparabilityThreshold(
      const double& additiveSeparabilityThreshold) {
    if(additiveSeparabilityThreshold < 0 || additiveSeparabilityThreshold > 1) {
      throw std::runtime_error("The additive separability threshold (" + std::to_string(additiveSeparabilityThreshold) + ") must be within 0 and 1.");
    }

    additiveSeparabilityThreshold_ = additiveSeparabilityThreshold;
  }
}
