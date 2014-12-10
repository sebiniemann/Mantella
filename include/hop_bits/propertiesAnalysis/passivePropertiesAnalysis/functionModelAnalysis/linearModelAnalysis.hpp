#pragma once

// HOP
#include <hop_bits/propertiesAnalysis/passivePropertiesAnalysis/functionModelAnalysis.hpp>

namespace hop {
  class LinearModelAnalysis : public FunctionModelAnalysis {
    public:
      using FunctionModelAnalysis::FunctionModelAnalysis;

      arma::Col<double> getLinearModelEstimator() const noexcept;

    protected:
      arma::Col<double> linearModelEstimator_;
  };
}
