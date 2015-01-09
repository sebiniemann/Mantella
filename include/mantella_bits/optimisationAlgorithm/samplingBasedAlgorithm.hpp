#pragma once

// Mantella
#include <mantella_bits/optimisationAlgorithm.hpp>

namespace mant {
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
