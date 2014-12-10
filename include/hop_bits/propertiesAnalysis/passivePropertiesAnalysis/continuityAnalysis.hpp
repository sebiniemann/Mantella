#pragma once

// HOP
#include <hop_bits/propertiesAnalysis/passivePropertiesAnalysis.hpp>

namespace hop {
  class ContinuityAnalysis : public PassivePropertiesAnalysis {
    public:
      using PassivePropertiesAnalysis::PassivePropertiesAnalysis;
  };
}
