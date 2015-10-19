#pragma once

// C++ standard library
#include <memory>
#include <string>

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/propertyAnalysis/activePropertyAnalysis.hpp"

namespace mant {
  class TranslationPeriodicityAnalysis : public ActivePropertyAnalysis {
   public:
    using ActivePropertyAnalysis::ActivePropertyAnalysis;

    void setMaximalNumberOfIterations(
        const arma::uword maximalNumberOfIterations);

    std::string toString() const override;

   protected:
    arma::uword maximalNumberOfIterations_;

    void analyseImplementation() override;
  };
}
