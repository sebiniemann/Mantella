#pragma once

// HOP
#include <hop_bits/propertiesAnalysis/passivePropertiesAnalysis.hpp>

namespace hop {
  template <typename ParameterType, class DistanceFunction>
  class ContinuityAnalysis : public PassivePropertiesAnalysis<ParameterType, DistanceFunction> {
    public:
      using PassivePropertiesAnalysis<ParameterType, DistanceFunction>::PassivePropertiesAnalysis;
  };

  //! Only FORWARD DECLARARTION of FULLY TEMPLATE SPECIALISATION from here on.
  //! Note: Forward declaration is needed to avoid ordering errors within the source file.

  // Nothing to see here, move along ...

  //! Only PARTIAL TEMPLATE SPECIALISATION from here on.
  //!
  //! Only PUBLIC methods from here on
  //! Note: Runtime checks are only performed for public methods.

  // Nothing to see here, move along ...

  //! ALL METHODS SHOULD BE EITHER PROTECTED OR PRIVATE FROM HERE ON
  //! Note: Runtime checks are only performed for public methods.

  // Nothing to see here, move along ...
}
