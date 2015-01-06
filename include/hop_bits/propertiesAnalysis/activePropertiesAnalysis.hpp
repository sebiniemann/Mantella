#pragma once

// HOP
#include <hop_bits/propertiesAnalysis.hpp>

namespace hop {
  template <typename ParameterType, class DistanceFunction>
  class ActivePropertiesAnalysis : public PropertiesAnalysis<ParameterType, DistanceFunction> {
    public:
      using PropertiesAnalysis<ParameterType, DistanceFunction>::PropertiesAnalysis;
  };
}
