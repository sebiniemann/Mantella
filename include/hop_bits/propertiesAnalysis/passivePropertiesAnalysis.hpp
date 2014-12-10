#pragma once

// C++ Standard Library
#include <utility>

// HOP
#include <hop_bits/propertiesAnalysis.hpp>

namespace hop {
  class PassivePropertiesAnalysis : public PropertiesAnalysis {
    public:
      using PropertiesAnalysis::PropertiesAnalysis;

      using PropertiesAnalysis::analyse;

      void analyse(
          const std::pair<arma::Col<double>, double>& parameterToObjectiveValueMapping) noexcept;
    protected:
      using PropertiesAnalysis::analyseImplementation;

      virtual void analyseImplementation(
          const std::pair<arma::Col<double>, double>& parameterToObjectiveValueMapping) noexcept = 0;
  };
}
