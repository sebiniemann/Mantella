#pragma once

// C++ standard library
#include <string>

// Mantella
#include "mantella_bits/propertyAnalysis/passivePropertyAnalysis.hpp"

namespace mant {
  class LipschitzContinuityAnalysis : public PassivePropertyAnalysis {
   public:
    using PassivePropertyAnalysis::PassivePropertyAnalysis;

    double getLipschitzConstant() const;

    std::string toString() const override;

   protected:
    double lipschitzConstant_;

    void analyseImplementation() override;
  };
}
