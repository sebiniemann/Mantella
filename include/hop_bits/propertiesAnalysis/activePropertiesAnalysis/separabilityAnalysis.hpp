#pragma once

// C++ Standard Library
#include <vector>

// HOP
#include <hop_bits/propertiesAnalysis/activePropertiesAnalysis.hpp>

namespace hop {
  class SeparabilityAnalysis : public ActivePropertiesAnalysis {
    public:
      using ActivePropertiesAnalysis::ActivePropertiesAnalysis;

      void setMaximalNumberOfIterations(
          const unsigned int& maximalNumberOfIterations) noexcept;

    protected:
      unsigned int maximalNumberOfIterations_;
  };
}
