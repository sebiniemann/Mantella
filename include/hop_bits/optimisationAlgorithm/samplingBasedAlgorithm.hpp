#pragma once

// HOP
#include <hop_bits/optimisationAlgorithm.hpp>

namespace hop {
  template <typename ParameterType>
  class SamplingBasedAlgorithm : public OptimisationAlgorithm<ParameterType> {
    public:
      explicit SamplingBasedAlgorithm(
          const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem) noexcept;
  };

  template <typename ParameterType>
  SamplingBasedAlgorithm<ParameterType>::SamplingBasedAlgorithm(
      const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem) noexcept
    : OptimisationAlgorithm<ParameterType>(optimisationProblem) {
  }
}
