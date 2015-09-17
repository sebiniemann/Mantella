#pragma once

// C++ standard library
#include <memory>

// Mantella
#include <mantella_bits/optimisationAlgorithm.hpp>

namespace mant {
  class SamplingBasedOptimisationAlgorithm : public OptimisationAlgorithm {
   public:
    using OptimisationAlgorithm::OptimisationAlgorithm;
  };
}
