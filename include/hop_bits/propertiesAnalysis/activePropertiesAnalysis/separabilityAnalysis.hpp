#pragma once

// C++ Standard Library
#include <vector>

// HOP
#include <hop_bits/propertiesAnalysis/activePropertiesAnalysis.hpp>

namespace hop {
  template <typename ParameterType, class DistanceFunction>
  class SeparabilityAnalysis : public ActivePropertiesAnalysis<ParameterType, DistanceFunction> {
    public:
      using ActivePropertiesAnalysis<ParameterType, DistanceFunction>::ActivePropertiesAnalysis;

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
