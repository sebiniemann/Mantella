#pragma once

// HOP
#include <hop_bits/propertiesAnalysis/passivePropertiesAnalysis/functionModelAnalysis.hpp>

namespace hop {
  template <typename ParameterType>
  class LinearModelAnalysis : public FunctionModelAnalysis<ParameterType> {
    public:
      explicit LinearModelAnalysis() noexcept;

      // Copy constructors are not used in this library and deleted to avoid unintended/any usage.
      LinearModelAnalysis(const LinearModelAnalysis&) = delete;
      LinearModelAnalysis& operator=(const LinearModelAnalysis&) = delete;

      void setLinearModelResidualsThreshold(
          const double& linearModelResidualsThreshold);

      arma::Col<double> getLinearModelEstimator() const noexcept;

      bool isLinear() const noexcept;

    protected:
      arma::Col<double> linearModelEstimator_;
      bool isLinear_;

      double linearModelResidualsThreshold_;
  };

  //! Only FORWARD DECLERARTION of FULLY TEMPLATE SPECIALISATION from here on.
  //! Note: Forward declaration is needed to avoid ordering errors within the source file.

  // Nothing to see here, move along ...

  //! Only PARTIAL TEMPLATE SPECIALISATION from here on.
  //!
  //! Only PUBLIC methods from here on
  //! Note: Runtime checks are only performed for public methods.

  template <typename ParameterType>
  LinearModelAnalysis<ParameterType>::LinearModelAnalysis() noexcept
    : isLinear_(false) {
    setLinearModelResidualsThreshold(0.25);
  }

  template <typename ParameterType>
  arma::Col<double> LinearModelAnalysis<ParameterType>::getLinearModelEstimator() const noexcept {
    return linearModelEstimator_;
  }

  template <typename ParameterType>
  void LinearModelAnalysis<ParameterType>::setLinearModelResidualsThreshold(
      const double& linearModelResidualsThreshold) {
    if(linearModelResidualsThreshold < 0 || linearModelResidualsThreshold > 1) {
      throw std::runtime_error("The quadratic model residuals threshold (" + std::to_string(linearModelResidualsThreshold) + ") must be within 0 and 1.");
    }

    linearModelResidualsThreshold_ = linearModelResidualsThreshold;
  }

  template <typename ParameterType>
  bool LinearModelAnalysis<ParameterType>::isLinear() const noexcept {
    return isLinear_;
  }
  //! ALL METHODS SHOULD BE EITHER PROTECTED OR PRIVATE FROM HERE ON
  //! Note: Runtime checks are only performed for public methods.

  // Nothing to see here, move along ...
}
