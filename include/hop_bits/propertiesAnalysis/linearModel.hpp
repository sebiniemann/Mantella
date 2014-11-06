#pragma once

// HOP
#include <hop_bits/propertiesAnalysis.hpp>

namespace hop {
  class LinearModel : public PropertiesAnalysis {
    public:
      arma::Col<double> getLinearEstimator() const;

    protected:
      arma::Col<double> linearEstimator_;
  };
}
