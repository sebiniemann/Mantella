#pragma once

// C++ standard library
#include <string>

// Mantella
#include "mantella_bits/propertyAnalysis/passivePropertyAnalysis.hpp"

namespace mant {
  class HoelderContinuityAnalysis : public PassivePropertyAnalysis {
   public:
    using PassivePropertyAnalysis::PassivePropertyAnalysis;

    double getHoelderExponent() const;

    std::string toString() const override;

   protected:
    double hoelderExponent_;

    void analyseImplementation() override;
  };
}
