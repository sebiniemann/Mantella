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

  //! Only FORWARD DECLERARTION of FULLY TEMPLATE SPECIALISATION from here on.
  //! Note: Forward declaration is needed to avoid ordering errors within the source file.

  // Nothing to see here, move along ...

  //! Only PARTIAL TEMPLATE SPECIALISATION from here on.
  //!
  //! Only PUBLIC methods from here on
  //! Note: Runtime checks are only performed for public methods.

  template <typename ParameterType>
  SamplingBasedAlgorithm<ParameterType>::SamplingBasedAlgorithm(
      const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem) noexcept
    : OptimisationAlgorithm<ParameterType>(optimisationProblem) {
  }

  //! ALL METHODS SHOULD BE EITHER PROTECTED OR PRIVATE FROM HERE ON
  //! Note: Runtime checks are only performed for public methods.

  // Nothing to see here, move along ...
}
