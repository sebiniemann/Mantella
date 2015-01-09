#pragma once

// Mantella
#include <mantella_bits/propertiesAnalysis/passivePropertiesAnalysis.hpp>

namespace mant {
  template <typename ParameterType, class DistanceFunction>
  class ContinuityAnalysis : public PassivePropertiesAnalysis<ParameterType, DistanceFunction> {
    public:
      using PassivePropertiesAnalysis<ParameterType, DistanceFunction>::PassivePropertiesAnalysis;
  };
}
