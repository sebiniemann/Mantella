#pragma once

// HOP
#include <hop_bits/propertiesAnalysis/passivePropertiesAnalysis/functionModelAnalysis.hpp>

namespace hop {
  template <typename ParameterType, class DistanceFunction>
  class QuadraticModelAnalysis : public FunctionModelAnalysis<ParameterType, DistanceFunction> {
    public:
      explicit QuadraticModelAnalysis() noexcept;

      // Copy constructors are not used in this library and deleted to avoid unintended/any usage.
      QuadraticModelAnalysis(const QuadraticModelAnalysis&) = delete;
      QuadraticModelAnalysis& operator=(const QuadraticModelAnalysis&) = delete;

      void setQuadraticModelMeanErrorThreshold(
          const double& quadraticModelMeanErrorThreshold);

      arma::Col<double> getQuadraticModelEstimator() const noexcept;

      bool isQuadratic() const noexcept;

    protected:
      arma::Col<double> quadraticModelEstimator_;
      bool isQuadratic_;

      double quadraticModelMeanErrorThreshold_;
  };

  template <typename ParameterType, class DistanceFunction>
  QuadraticModelAnalysis<ParameterType, DistanceFunction>::QuadraticModelAnalysis() noexcept
    : isQuadratic_(false) {
    setQuadraticModelMeanErrorThreshold(0.25);
  }

  template <typename ParameterType, class DistanceFunction>
  arma::Col<double> QuadraticModelAnalysis<ParameterType, DistanceFunction>::getQuadraticModelEstimator() const noexcept {
    return quadraticModelEstimator_;
  }

  template <typename ParameterType, class DistanceFunction>
  void QuadraticModelAnalysis<ParameterType, DistanceFunction>::setQuadraticModelMeanErrorThreshold(
      const double& quadraticModelMeanErrorThreshold) {
    if(quadraticModelMeanErrorThreshold < 0 || quadraticModelMeanErrorThreshold > 1) {
      throw std::runtime_error("The quadratic model MeanError threshold (" + std::to_string(quadraticModelMeanErrorThreshold) + ") must be within 0 and 1.");
    }

    quadraticModelMeanErrorThreshold_ = quadraticModelMeanErrorThreshold;
  }

  template <typename ParameterType, class DistanceFunction>
  bool QuadraticModelAnalysis<ParameterType, DistanceFunction>::isQuadratic() const noexcept {
    return isQuadratic_;
  }
}
