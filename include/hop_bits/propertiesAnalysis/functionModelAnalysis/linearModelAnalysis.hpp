#pragma once

// HOP
#include <hop_bits/propertiesAnalysis/functionModelAnalysis.hpp>

namespace hop {
  class LinearModelAnalysis : public FunctionModelAnalysis {
    public:
      arma::Col<double> getLinearModelEstimator() const noexcept;

    protected:
      arma::Col<double> linearModelEstimator_;
  };
}
