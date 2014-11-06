#pragma once

// HOP
#include <hop_bits/propertiesAnalysis.hpp>

namespace hop {
  class QuadraticModel : public PropertiesAnalysis {
    public:
      arma::Col<double> getQuadraticEstimator();

    protected:
      arma::Col<double> quadraticEstimator_;
  };
}
