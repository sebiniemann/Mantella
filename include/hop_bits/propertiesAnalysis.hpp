#pragma once

// C++ Standard Library
#include <memory>

// HOP
#include <hop_bits/optimisationProblem.hpp>

namespace hop {
  class PropertiesAnalysis {
    public:
      explicit PropertiesAnalysis() noexcept;

      // Copy constructors are not used in this library and deleted to avoid unintended/any usage.
      PropertiesAnalysis(const PropertiesAnalysis&) = delete;
      PropertiesAnalysis& operator=(const PropertiesAnalysis&) = delete;

      void analyse(
          const std::shared_ptr<OptimisationProblem> optimisationProblem) noexcept;

      double getPlausibility() const noexcept;

    protected:
      double plausibility_;

      virtual void analyseImplementation(
          const std::shared_ptr<OptimisationProblem> optimisationProblem) noexcept = 0;
  };
}
