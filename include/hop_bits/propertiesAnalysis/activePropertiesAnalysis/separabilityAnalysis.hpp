#pragma once

// C++ Standard Library
#include <vector>

// HOP
#include <hop_bits/propertiesAnalysis/activePropertiesAnalysis.hpp>

namespace hop {
  template <typename ParameterType, class DistanceFunction>
  class SeparabilityAnalysis : public ActivePropertiesAnalysis<ParameterType, DistanceFunction> {
    public:
      explicit SeparabilityAnalysis() noexcept
        : maximalNumberOfIterations_(1000.0) {

      }

      // Copy constructors are not used in this library and deleted to avoid unintended/any usage.
      SeparabilityAnalysis(const SeparabilityAnalysis&) = delete;
      SeparabilityAnalysis& operator=(const SeparabilityAnalysis&) = delete;

      void setMaximalNumberOfIterations(
          const unsigned int& maximalNumberOfIterations) noexcept;

    protected:
      unsigned int maximalNumberOfIterations_;
  };

  template <typename ParameterType, class DistanceFunction>
  void SeparabilityAnalysis<ParameterType, DistanceFunction>::setMaximalNumberOfIterations(
      const unsigned int& maximalNumberOfIterations) noexcept {
    maximalNumberOfIterations_ = maximalNumberOfIterations;
  }
}
