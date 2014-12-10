#pragma once

// HOP
#include <hop_bits/propertiesAnalysis/activePropertiesAnalysis/separabilityAnalysis.hpp>

namespace hop {
  class AdditiveSeparabilityAnalysis : public SeparabilityAnalysis {
    public:
      using SeparabilityAnalysis::SeparabilityAnalysis;
  };
}
