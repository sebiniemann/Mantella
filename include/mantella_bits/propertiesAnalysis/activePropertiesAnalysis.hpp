#pragma once

// Mantella
#include <mantella_bits/propertiesAnalysis.hpp>

namespace mant {
  template <typename ParameterType, class DistanceFunction>
  class ActivePropertiesAnalysis : public PropertiesAnalysis<ParameterType, DistanceFunction> {
    public:
      using PropertiesAnalysis<ParameterType, DistanceFunction>::PropertiesAnalysis;
  };
}
