#pragma once

// HOP
#include <hop_bits/propertiesAnalysis.hpp>

namespace hop {
  class FunctionModelAnalysis : public PropertiesAnalysis {
    public:
      arma::Col<double> getResiduals() const;

    protected:
      arma::Col<double> residuals_;
  };
}
