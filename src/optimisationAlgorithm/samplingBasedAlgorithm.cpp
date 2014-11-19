#include <hop_bits/optimisationAlgorithm/samplingBasedAlgorithm.hpp>

namespace hop {
  SamplingBasedAlgorithm::SamplingBasedAlgorithm(
      const std::shared_ptr<OptimisationProblem> optimisationProblem)
    : OptimisationAlgorithm(optimisationProblem) {
  }
}
