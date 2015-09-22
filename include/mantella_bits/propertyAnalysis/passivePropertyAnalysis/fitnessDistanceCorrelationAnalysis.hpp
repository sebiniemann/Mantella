#pragma once

// C++ standard library
#include <string>

// Mantella
#include <mantella_bits/propertyAnalysis/passivePropertyAnalysis.hpp>

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
