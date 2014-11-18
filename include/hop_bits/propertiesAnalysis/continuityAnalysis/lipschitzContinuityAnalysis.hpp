#pragma once

// HOP
#include <hop_bits/propertiesAnalysis/continuityAnalysis.hpp>

namespace hop {
  class LipschitzContinuityAnalysis : public ContinuityAnalysis {
    public:
      double getLipschitzConstant() const noexcept;

    protected:
      double lipschitzConstant_;
  };
}
