#pragma once

// HOP
#include <hop_bits/propertiesAnalysis.hpp>

namespace hop {
  class FunctionModelAnalysis : public PropertiesAnalysis {
    public:
      using PropertiesAnalysis::PropertiesAnalysis;

      arma::Col<double> getResiduals() const noexcept;

    protected:
      arma::Col<double> residuals_;
  };
}
