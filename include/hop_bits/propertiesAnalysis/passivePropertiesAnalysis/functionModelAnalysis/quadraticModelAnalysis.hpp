#pragma once

// HOP
#include <hop_bits/propertiesAnalysis/passivePropertiesAnalysis/functionModelAnalysis.hpp>

namespace hop {
  class QuadraticModelAnalysis : public FunctionModelAnalysis {
    public:
      explicit QuadraticModelAnalysis() noexcept;

      // Copy constructors are not used in this library and deleted to avoid unintended/any usage.
      QuadraticModelAnalysis(const QuadraticModelAnalysis&) = delete;
      QuadraticModelAnalysis& operator=(const QuadraticModelAnalysis&) = delete;

      void setQuadraticModelResidualsThreshold(
          const double& quadraticModelResidualsThreshold);

      arma::Col<double> getQuadraticModelEstimator() const noexcept;

    protected:
      arma::Col<double> quadraticModelEstimator_;

      double quadraticModelResidualsThreshold_;
  };
}
