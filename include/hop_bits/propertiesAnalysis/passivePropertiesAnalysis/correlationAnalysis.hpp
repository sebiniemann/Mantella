#pragma once

// HOP
#include <hop_bits/propertiesAnalysis/passivePropertiesAnalysis.hpp>

namespace hop {
  template <typename ParameterType, class DistanceFunction = EuclideanDistance>
    class CorrelationAnalysis : public PassivePropertiesAnalysis<ParameterType, DistanceFunction> {
    public:
      explicit CorrelationAnalysis() noexcept;

      CorrelationAnalysis(const CorrelationAnalysis&) = delete;
      CorrelationAnalysis& operator=(const CorrelationAnalysis&) = delete;

      double getCorrelationCoefficient() const noexcept;

    protected:
      double correlationCoefficient_;
  };

  //! Only FORWARD DECLERARTION of FULLY TEMPLATE SPECIALISATION from here on.
  //! Note: Forward declaration is needed to avoid ordering errors within the source file.

  // Nothing to see here, move along ...

  //! Only PARTIAL TEMPLATE SPECIALISATION from here on.
  //!
  //! Only PUBLIC methods from here on
  //! Note: Runtime checks are only performed for public methods.

  template <typename ParameterType, class DistanceFunction>
  CorrelationAnalysis<ParameterType, DistanceFunction>::CorrelationAnalysis() noexcept
    : correlationCoefficient_(0.0) {

  }

  template <typename ParameterType, class DistanceFunction>
  double CorrelationAnalysis<ParameterType, DistanceFunction>::getCorrelationCoefficient() const noexcept {
    return correlationCoefficient_;
  }

  //! ALL METHODS SHOULD BE EITHER PROTECTED OR PRIVATE FROM HERE ON
  //! Note: Runtime checks are only performed for public methods.

  // Nothing to see here, move along ...
}
