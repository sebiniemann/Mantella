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
      explicit LinearFunctionModelAnalysis(
          const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples);

      arma::Col<double> getCoefficients() const;
      double getErrorTerm() const;

      arma::Col<double> getResiduals() const;
      
      std::string toString() const override;

    protected:
      arma::Col<double> coefficients_;
      double errorTerm_;
      
      arma::Col<double> residuals_;

      void analyseImplementation() override;
  };
}
