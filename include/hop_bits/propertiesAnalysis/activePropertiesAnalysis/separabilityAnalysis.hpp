#pragma once

// C++ Standard Library
#include <vector>

// HOP
#include <hop_bits/propertiesAnalysis/activePropertiesAnalysis.hpp>

namespace hop {
  class SeparabilityAnalysis : public ActivePropertiesAnalysis {
    public:
      using ActivePropertiesAnalysis::ActivePropertiesAnalysis;
  };
}
