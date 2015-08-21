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
  class CubicFunctionModelAnalysis : public PassivePropertyAnalysis {
    public:
      explicit CubicFunctionModelAnalysis(
          const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples);

      arma::Cube<double> getCubicCoefficients() const;
      arma::Mat<double> getQuadraticCoefficients() const;
      arma::Col<double> getLinearCoefficients() const;
      double getErrorTerm() const;
      
      arma::Col<double> getResiduals() const;

      std::string toString() const override;

    protected:
      arma::Cube<double> cubicCoefficients_;
      arma::Mat<double> quadraticCoefficients_;
      arma::Col<double> linearCoefficients_;
      double errorTerm_;
      
      arma::Col<double> residuals_;

      void analyseImplementation() override;
  };
}
