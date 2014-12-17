#pragma once

// HOP
#include <hop_bits/propertiesAnalysis/activePropertiesAnalysis.hpp>

namespace hop {
  template <typename ParameterType>
  class ProportionalityAnalysis : public ActivePropertiesAnalysis<ParameterType> {
    public:
      using ActivePropertiesAnalysis<ParameterType>::ActivePropertiesAnalysis;

      void setMaximalNumberOfIterations(
          const unsigned int& maximalNumberOfIterations) noexcept;

    protected:
      unsigned int maximalNumberOfIterations_;
  };

  //! Only FORWARD DECLERARTION of FULLY TEMPLATE SPECIALISATION from here on.
  //! Note: Forward declaration is needed to avoid ordering errors within the source file.

  // Nothing to see here, move along ...

  //! Only PARTIAL TEMPLATE SPECIALISATION from here on.
  //!
  //! Only PUBLIC methods from here on
  //! Note: Runtime checks are only performed for public methods.

  template <typename ParameterType>
  void ProportionalityAnalysis<ParameterType>::setMaximalNumberOfIterations(
      const unsigned int& maximalNumberOfIterations) noexcept {
    maximalNumberOfIterations_ = maximalNumberOfIterations;
  }

  //! ALL METHODS SHOULD BE EITHER PROTECTED OR PRIVATE FROM HERE ON
  //! Note: Runtime checks are only performed for public methods.

  // Nothing to see here, move along ...
}
