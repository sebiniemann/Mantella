#pragma once

// C++ Standard Library
#include <utility>

// HOP
#include <hop_bits/propertiesAnalysis.hpp>

namespace hop {
  template <typename ParameterType, class DistanceFunction>
  class PassivePropertiesAnalysis : public PropertiesAnalysis<ParameterType, DistanceFunction> {
    public:
      using PropertiesAnalysis<ParameterType, DistanceFunction>::PropertiesAnalysis;

      using PropertiesAnalysis<ParameterType, DistanceFunction>::analyse;

      void analyse(
          const std::pair<arma::Col<ParameterType>, double>& parameterToObjectiveValueMapping) noexcept;
    protected:
      using PropertiesAnalysis<ParameterType, DistanceFunction>::analyseImplementation;

      virtual void analyseImplementation(
          const std::pair<arma::Col<ParameterType>, double>& parameterToObjectiveValueMapping) noexcept = 0;
  };

  //! Only FORWARD DECLARARTION of FULLY TEMPLATE SPECIALISATION from here on.
  //! Note: Forward declaration is needed to avoid ordering errors within the source file.

  // Nothing to see here, move along ...

  //! Only PARTIAL TEMPLATE SPECIALISATION from here on.
  //!
  //! Only PUBLIC methods from here on
  //! Note: Runtime checks are only performed for public methods.

  template <typename ParameterType, class DistanceFunction>
  void PassivePropertiesAnalysis<ParameterType, DistanceFunction>::analyse(
      const std::pair<arma::Col<ParameterType>, double>& parameterToObjectiveValueMapping) noexcept {
    PropertiesAnalysis<ParameterType, DistanceFunction>::plausibility_ = 0.0;

    analyseImplementation(parameterToObjectiveValueMapping);
  }

  //! ALL METHODS SHOULD BE EITHER PROTECTED OR PRIVATE FROM HERE ON
  //! Note: Runtime checks are only performed for public methods.

  // Nothing to see here, move along ...
}
