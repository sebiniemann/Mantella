#pragma once

// HOP
#include <hop_bits/propertiesAnalysis/activePropertiesAnalysis/separabilityAnalysis.hpp>

namespace hop {
  class AdditiveSeparabilityAnalysis : public SeparabilityAnalysis {
    public:
      explicit AdditiveSeparabilityAnalysis() noexcept;

      // Copy constructors are not used in this library and deleted to avoid unintended/any usage.
      AdditiveSeparabilityAnalysis(const AdditiveSeparabilityAnalysis&) = delete;
      AdditiveSeparabilityAnalysis& operator=(const AdditiveSeparabilityAnalysis&) = delete;

      void setAdditiveSeparabilityThreshold(
          const double& additiveSeparabilityThreshold);

    protected:
      double additiveSeparabilityThreshold_;
  };
}
