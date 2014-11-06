#pragma once

// HOP
#include <hop_bits/propertiesAnalysis/quadraticModel.hpp>

namespace hop {
  class QuadraticOrdinaryLeastSquares : public QuadraticModel {
    protected:
      void analyseImplementation(
          const arma::Mat<double>& parameters,
          const arma::Col<double>& objectiveValues) override;
  };
}
