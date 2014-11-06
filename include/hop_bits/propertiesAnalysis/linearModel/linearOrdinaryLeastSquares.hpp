#pragma once

// HOP
#include <hop_bits/propertiesAnalysis/linearModel.hpp>

namespace hop {
  class LinearOrdinaryLeastSquares : public LinearModel {
    protected:
      void analyseImplementation(
          const arma::Mat<double>& parameters,
          const arma::Col<double>& objectiveValues) override;
  };
}
