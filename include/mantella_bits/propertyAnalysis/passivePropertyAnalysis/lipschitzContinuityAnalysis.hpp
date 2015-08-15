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
