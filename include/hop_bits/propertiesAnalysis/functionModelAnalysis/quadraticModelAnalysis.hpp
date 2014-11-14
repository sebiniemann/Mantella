#pragma once

// HOP
#include <hop_bits/propertiesAnalysis/functionModelAnalysis.hpp>

namespace hop {
  class QuadraticModelAnalysis : public FunctionModelAnalysis {
    public:
      arma::Col<double> getQuadraticModelEstimator() const;

    protected:
      arma::Col<double> quadraticModelEstimator_;
  };
}
