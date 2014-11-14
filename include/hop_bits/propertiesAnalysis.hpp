#pragma once

// C++ Standard Library
#include <memory>

// HOP
#include <hop_bits/optimisationProblem.hpp>

namespace hop {
  class PropertiesAnalysis {
    public:
      explicit PropertiesAnalysis();

      // Copy constructors are not used in this library and deleted to avoid unintended/any usage.
      PropertiesAnalysis(const PropertiesAnalysis&) = delete;
      PropertiesAnalysis& operator=(const PropertiesAnalysis&) = delete;

      void analyse(
          const std::shared_ptr<OptimisationProblem> optimisationProblem);

      double getPlausibility() const;

    protected:
      double plausibility_;

      virtual void analyseImplementation(
          const std::shared_ptr<OptimisationProblem> optimisationProblem) = 0;
  };
}
