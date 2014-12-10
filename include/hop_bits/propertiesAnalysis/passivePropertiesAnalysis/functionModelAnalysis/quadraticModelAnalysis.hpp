#pragma once

// HOP
#include <hop_bits/propertiesAnalysis/passivePropertiesAnalysis/functionModelAnalysis.hpp>

namespace hop {
  class QuadraticModelAnalysis : public FunctionModelAnalysis {
    public:
      using FunctionModelAnalysis::FunctionModelAnalysis;

      arma::Col<double> getQuadraticModelEstimator() const noexcept;

    protected:
      arma::Col<double> quadraticModelEstimator_;
  };
}
