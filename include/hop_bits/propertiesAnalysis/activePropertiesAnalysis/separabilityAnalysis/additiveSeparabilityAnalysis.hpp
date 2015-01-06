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

      void setAdditiveSeparabilityMeanErrorThreshold(
          const double& additiveSeparabilityMeanErrorThreshold);

    protected:
      double additiveSeparabilityMeanErrorThreshold_;
  };

  template <typename ParameterType, class DistanceFunction>
  AdditiveSeparabilityAnalysis<ParameterType, DistanceFunction>::AdditiveSeparabilityAnalysis() noexcept {
    setAdditiveSeparabilityMeanErrorThreshold(0.75);
  }

  template <typename ParameterType, class DistanceFunction>
  void AdditiveSeparabilityAnalysis<ParameterType, DistanceFunction>::setAdditiveSeparabilityMeanErrorThreshold(
      const double& additiveSeparabilityMeanErrorThreshold) {
    if(additiveSeparabilityMeanErrorThreshold < 0 || additiveSeparabilityMeanErrorThreshold > 1) {
      throw std::runtime_error("The additive separability error threshold (" + std::to_string(additiveSeparabilityMeanErrorThreshold) + ") must be within 0 and 1.");
    }

    additiveSeparabilityMeanErrorThreshold_ = additiveSeparabilityMeanErrorThreshold;
  }
}
