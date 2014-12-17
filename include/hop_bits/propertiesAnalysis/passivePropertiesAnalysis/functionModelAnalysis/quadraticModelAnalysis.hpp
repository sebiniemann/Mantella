#pragma once

// HOP
#include <hop_bits/propertiesAnalysis/passivePropertiesAnalysis/functionModelAnalysis.hpp>

namespace hop {
  template <typename ParameterType>
  class QuadraticModelAnalysis : public FunctionModelAnalysis<ParameterType> {
    public:
      explicit QuadraticModelAnalysis() noexcept;

      // Copy constructors are not used in this library and deleted to avoid unintended/any usage.
      QuadraticModelAnalysis(const QuadraticModelAnalysis&) = delete;
      QuadraticModelAnalysis& operator=(const QuadraticModelAnalysis&) = delete;

      void setQuadraticModelResidualsThreshold(
          const double& quadraticModelResidualsThreshold);

      arma::Col<double> getQuadraticModelEstimator() const noexcept;

      bool isQuadratic() const noexcept;

    protected:
      arma::Col<double> quadraticModelEstimator_;
      bool isQuadratic_;

      double quadraticModelResidualsThreshold_;
  };

  //! Only FORWARD DECLERARTION of FULLY TEMPLATE SPECIALISATION from here on.
  //! Note: Forward declaration is needed to avoid ordering errors within the source file.

  // Nothing to see here, move along ...

  //! Only PARTIAL TEMPLATE SPECIALISATION from here on.
  //!
  //! Only PUBLIC methods from here on
  //! Note: Runtime checks are only performed for public methods.

  template <typename ParameterType>
  QuadraticModelAnalysis<ParameterType>::QuadraticModelAnalysis() noexcept
    : isQuadratic_(false) {
    setQuadraticModelResidualsThreshold(0.25);
  }

  template <typename ParameterType>
  arma::Col<double> QuadraticModelAnalysis<ParameterType>::getQuadraticModelEstimator() const noexcept {
    return quadraticModelEstimator_;
  }

  template <typename ParameterType>
  void QuadraticModelAnalysis<ParameterType>::setQuadraticModelResidualsThreshold(
      const double& quadraticModelResidualsThreshold) {
    if(quadraticModelResidualsThreshold < 0 || quadraticModelResidualsThreshold > 1) {
      throw std::runtime_error("The quadratic model residuals threshold (" + std::to_string(quadraticModelResidualsThreshold) + ") must be within 0 and 1.");
    }

    quadraticModelResidualsThreshold_ = quadraticModelResidualsThreshold;
  }

  template <typename ParameterType>
  bool QuadraticModelAnalysis<ParameterType>::isQuadratic() const noexcept {
    return isQuadratic_;
  }

  //! ALL METHODS SHOULD BE EITHER PROTECTED OR PRIVATE FROM HERE ON
  //! Note: Runtime checks are only performed for public methods.

  // Nothing to see here, move along ...
}
