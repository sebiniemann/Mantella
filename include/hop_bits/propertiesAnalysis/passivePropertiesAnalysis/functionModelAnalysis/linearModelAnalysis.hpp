#pragma once

// HOP
#include <hop_bits/propertiesAnalysis/passivePropertiesAnalysis/functionModelAnalysis.hpp>

namespace hop {
  class LinearModelAnalysis : public FunctionModelAnalysis {
    public:
      explicit LinearModelAnalysis() noexcept;

      // Copy constructors are not used in this library and deleted to avoid unintended/any usage.
      LinearModelAnalysis(const LinearModelAnalysis&) = delete;
      LinearModelAnalysis& operator=(const LinearModelAnalysis&) = delete;

      void setLinearModelResidualsThreshold(
          const double& linearModelResidualsThreshold);

      arma::Col<double> getLinearModelEstimator() const noexcept;

    protected:
      arma::Col<double> linearModelEstimator_;

      double linearModelResidualsThreshold_;
  };
}
