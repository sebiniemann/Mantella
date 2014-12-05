#pragma once

// HOP
#include <hop_bits/propertiesAnalysis/continuityAnalysis.hpp>

namespace hop {
  class LipschitzContinuityAnalysis : public ContinuityAnalysis {
    public:
      explicit LipschitzContinuityAnalysis() noexcept;

      LipschitzContinuityAnalysis(const LipschitzContinuityAnalysis&) = delete;
      LipschitzContinuityAnalysis& operator=(const LipschitzContinuityAnalysis&) = delete;

      double getLipschitzConstant() const noexcept;

    protected:
      double lipschitzConstant_;
  };
}
