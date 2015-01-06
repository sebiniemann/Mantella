#pragma once

// HOP
#include <hop_bits/propertiesAnalysis/passivePropertiesAnalysis/continuityAnalysis.hpp>

namespace hop {
  template <typename ParameterType, class DistanceFunction>
  class LipschitzContinuityAnalysis : public ContinuityAnalysis<ParameterType, DistanceFunction> {
    public:
      explicit LipschitzContinuityAnalysis() noexcept;

      LipschitzContinuityAnalysis(const LipschitzContinuityAnalysis&) = delete;
      LipschitzContinuityAnalysis& operator=(const LipschitzContinuityAnalysis&) = delete;

      double getLipschitzConstant() const noexcept;

    protected:
      double lipschitzConstant_;
  };

  template <typename ParameterType, class DistanceFunction>
  LipschitzContinuityAnalysis<ParameterType, DistanceFunction>::LipschitzContinuityAnalysis() noexcept
    : lipschitzConstant_(0.0) {

  }

  template <typename ParameterType, class DistanceFunction>
  double LipschitzContinuityAnalysis<ParameterType, DistanceFunction>::getLipschitzConstant() const noexcept {
    return lipschitzConstant_;
  }
}
