#pragma once

// HOP
#include <hop_bits/propertiesAnalysis.hpp>

namespace hop {
  class LipschitzContinuityAnalysis : public PropertiesAnalysis {
    public:
      double getLipschitzConstant() const;

    protected:
      double lipschitzConstant_;
  };
}
