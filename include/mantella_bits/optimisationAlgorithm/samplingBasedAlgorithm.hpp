#pragma once

// Mantella
#include <mantella_bits/optimisationAlgorithm.hpp>

namespace mant {
  template <typename ParameterType, class DistanceFunction>
  class SamplingBasedAlgorithm : public OptimisationAlgorithm<ParameterType, DistanceFunction> {
    public:
      using OptimisationAlgorithm<ParameterType, DistanceFunction>::OptimisationAlgorithm;
  };
}
