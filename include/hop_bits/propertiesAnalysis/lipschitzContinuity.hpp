#pragma once

// HOP
#include <hop_bits/propertiesAnalysis.hpp>

namespace hop {
  class LipschitzContinuity : public PropertiesAnalysis {
    public:
      double getLipschitzConstant() const;

    protected:
      double lipschitzConstant_;
  };
}
