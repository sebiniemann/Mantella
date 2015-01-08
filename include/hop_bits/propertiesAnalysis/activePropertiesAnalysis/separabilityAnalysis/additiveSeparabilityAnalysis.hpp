#pragma once

// HOP
#include <hop_bits/propertiesAnalysis/activePropertiesAnalysis/separabilityAnalysis.hpp>

namespace hop {
  template <typename ParameterType, class DistanceFunction>
  class AdditiveSeparabilityAnalysis : public SeparabilityAnalysis<ParameterType, DistanceFunction> {
    public:
      explicit AdditiveSeparabilityAnalysis() noexcept;

      // Copy constructors are not used in this library and deleted to avoid unintended/any usage.
      AdditiveSeparabilityAnalysis(const AdditiveSeparabilityAnalysis&) = delete;
      AdditiveSeparabilityAnalysis& operator=(const AdditiveSeparabilityAnalysis&) = delete;

      void setAdditiveSeparabilityMedianErrorThreshold(
          const double& additiveSeparabilityMedianErrorThreshold);

    protected:
      double additiveSeparabilityMedianErrorThreshold_;
  };

  template <typename ParameterType, class DistanceFunction>
  AdditiveSeparabilityAnalysis<ParameterType, DistanceFunction>::AdditiveSeparabilityAnalysis() noexcept {
    setAdditiveSeparabilityMedianErrorThreshold(0.75);
  }

  template <typename ParameterType, class DistanceFunction>
  void AdditiveSeparabilityAnalysis<ParameterType, DistanceFunction>::setAdditiveSeparabilityMedianErrorThreshold(
      const double& additiveSeparabilityMedianErrorThreshold) {
    if(additiveSeparabilityMedianErrorThreshold < 0 || additiveSeparabilityMedianErrorThreshold > 1) {
      throw std::runtime_error("The additive separability error threshold (" + std::to_string(additiveSeparabilityMedianErrorThreshold) + ") must be within 0 and 1.");
    }

    additiveSeparabilityMedianErrorThreshold_ = additiveSeparabilityMedianErrorThreshold;
  }
}
