#pragma once

// HOP
#include <hop_bits/propertiesAnalysis/activePropertiesAnalysis/proportionalityAnalysis.hpp>

namespace hop {
  template <typename ParameterType, class DistanceFunction>
  class LinearProportionalityAnalysis : public ProportionalityAnalysis<ParameterType, DistanceFunction> {
    public:
      using ProportionalityAnalysis<ParameterType, DistanceFunction>::ProportionalityAnalysis;
  };
}
