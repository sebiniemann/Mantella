#pragma once

// C++ standard library
#include <unordered_map>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella_bits/propertyAnalysis.hpp>
#include <mantella_bits/helper/unorderedContainer.hpp>

namespace mant {
  class PassivePropertyAnalysis : public PropertyAnalysis {
   public:
    explicit PassivePropertyAnalysis(
        const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples);

    void analyse();

   protected:
    const arma::uword numberOfDimensions_;

    std::unordered_map<arma::Col<double>, double, Hash, IsEqual> samples_;

    virtual void analyseImplementation() = 0;
  };
}
