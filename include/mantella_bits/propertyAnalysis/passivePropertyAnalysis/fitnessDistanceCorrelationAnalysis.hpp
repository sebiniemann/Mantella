#pragma once

// C++ standard library
#include <unordered_map>
#include <string>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella_bits/propertyAnalysis/passivePropertyAnalysis.hpp>
#include <mantella_bits/helper/unorderedContainer.hpp>

namespace mant {
  class FitnessDistanceCorrelationAnalysis : public PassivePropertyAnalysis {
    public:
      using PassivePropertyAnalysis::PassivePropertyAnalysis;

      double getCorrelationCoefficient() const;

      std::string toString() const override;

    protected:
      double correlationCoefficient_;

      void analyseImplementation() override;
  };
}
