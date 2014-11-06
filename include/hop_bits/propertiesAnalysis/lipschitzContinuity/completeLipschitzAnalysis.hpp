#pragma once

// HOP
#include <hop_bits/propertiesAnalysis/lipschitzContinuity.hpp>

namespace hop {
  class CompleteLipschitzAnalysis : public LipschitzContinuity {
    protected:
      void analyseImplementation(
          const arma::Mat<double>& parameters,
          const arma::Col<double>& objectiveValues) override;
  };
}
