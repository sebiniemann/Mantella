#pragma once

// HOP
#include <hop_bits/propertiesAnalysis/passivePropertiesAnalysis/continuityAnalysis.hpp>

namespace hop {
  template <typename ParameterType>
  class LipschitzContinuityAnalysis : public ContinuityAnalysis<ParameterType> {
    public:
      explicit LipschitzContinuityAnalysis() noexcept;

      LipschitzContinuityAnalysis(const LipschitzContinuityAnalysis&) = delete;
      LipschitzContinuityAnalysis& operator=(const LipschitzContinuityAnalysis&) = delete;

      double getLipschitzConstant() const noexcept;

    protected:
      double lipschitzConstant_;
  };

  //! Only FORWARD DECLERARTION of FULLY TEMPLATE SPECIALISATION from here on.
  //! Note: Forward declaration is needed to avoid ordering errors within the source file.

  // Nothing to see here, move along ...

  //! Only PARTIAL TEMPLATE SPECIALISATION from here on.
  //!
  //! Only PUBLIC methods from here on
  //! Note: Runtime checks are only performed for public methods.

  template <typename ParameterType>
  LipschitzContinuityAnalysis<ParameterType>::LipschitzContinuityAnalysis() noexcept
    : lipschitzConstant_(0.0) {

  }

  template <typename ParameterType>
  double LipschitzContinuityAnalysis<ParameterType>::getLipschitzConstant() const noexcept {
    return lipschitzConstant_;
  }

  //! ALL METHODS SHOULD BE EITHER PROTECTED OR PRIVATE FROM HERE ON
  //! Note: Runtime checks are only performed for public methods.

  // Nothing to see here, move along ...
}
