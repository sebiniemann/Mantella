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
