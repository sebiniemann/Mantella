#pragma once

// HOP
#include <hop_bits/optimisationAlgorithm.hpp>

namespace hop {
  class SamplingBasedAlgorithm : public OptimisationAlgorithm {
    public:
      explicit SamplingBasedAlgorithm(
          const std::shared_ptr<OptimisationProblem> optimisationProblem);
  };
}
