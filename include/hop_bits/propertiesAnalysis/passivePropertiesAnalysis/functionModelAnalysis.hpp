#pragma once

// HOP
#include <hop_bits/propertiesAnalysis/passivePropertiesAnalysis.hpp>

namespace hop {
  class FunctionModelAnalysis : public PassivePropertiesAnalysis {
    public:
      using PassivePropertiesAnalysis::PassivePropertiesAnalysis;

      arma::Col<double> getResiduals() const noexcept;

    protected:
      arma::Col<double> residuals_;
  };
}
