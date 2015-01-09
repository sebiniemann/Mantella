#pragma once

// C++ Standard Library
#include <utility>

// Mantella
#include <mantella_bits/propertiesAnalysis.hpp>

namespace mant {
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

  template <typename ParameterType, class DistanceFunction>
  void PassivePropertiesAnalysis<ParameterType, DistanceFunction>::analyse(
      const std::pair<arma::Col<ParameterType>, double>& parameterToObjectiveValueMapping) noexcept {
    PropertiesAnalysis<ParameterType, DistanceFunction>::plausibility_ = 0.0;

    analyseImplementation(parameterToObjectiveValueMapping);
  }
}
