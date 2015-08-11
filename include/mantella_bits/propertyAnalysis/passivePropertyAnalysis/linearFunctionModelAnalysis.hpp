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
  class LinearFunctionModelAnalysis : public PassivePropertyAnalysis {
    public:
      using PassivePropertyAnalysis::PassivePropertyAnalysis;

      arma::Col<double> getCoefficients() const;
      double getErrorTerm() const;

      std::string toString() const override;

    protected:
      arma::Col<double> coefficients_;
      double errorTerm_;

      void analyseImplementation() override;
  };
}
