#pragma once

#include <hop_bits/evaluation.hpp>

namespace hop {
  class PerformanceProfile : public Evaluation {
    public:
      PerformanceProfile() = default;

      PerformanceProfile(const PerformanceProfile&) = delete;
      PerformanceProfile& operator=(const PerformanceProfile&) = delete;

    protected:
      void runImpelementation();
  };
}
