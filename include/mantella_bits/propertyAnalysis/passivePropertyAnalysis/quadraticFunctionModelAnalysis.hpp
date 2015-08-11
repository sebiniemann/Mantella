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
  class QuadraticFunctionModelAnalysis : public PassivePropertyAnalysis {
    public:
      explicit QuadraticFunctionModelAnalysis(
          const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples);

      arma::Mat<double> getQuadraticCoefficients() const;
      arma::Col<double> getLinearCoefficients() const;
      double getErrorTerm() const;

      std::string toString() const override;

    protected:
      arma::Mat<double> quadraticCoefficients_;
      arma::Col<double> linearCoefficients_;
      double errorTerm_;

      void analyseImplementation() override;
  };
}
