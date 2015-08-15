#pragma once

// C++ standard library
#include <memory>
#include <string>

// Mantella
#include <mantella_bits/propertyAnalysis/activePropertyAnalysis.hpp>
#include <mantella_bits/optimisationProblem.hpp>

namespace mant {
  class LinearProportionalityAnalysis : public ActivePropertyAnalysis {
    public:
      using ActivePropertyAnalysis::ActivePropertyAnalysis;

      std::string toString() const override;
      
    protected:
      void analyseImplementation() override;
  };
}
