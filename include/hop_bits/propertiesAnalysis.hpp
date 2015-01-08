#pragma once

// C++ Standard Library
#include <memory>

// HOP
#include <hop_bits/optimisationProblem.hpp>

namespace hop {
  template <typename ParameterType, class DistanceFunction>
  class PropertiesAnalysis {
    public:
      explicit PropertiesAnalysis() noexcept
        : plausibility_(0.0) {

      }

      // Copy constructors are not used in this library and deleted to avoid unintended/any usage.
      PropertiesAnalysis(const PropertiesAnalysis&) = delete;
      PropertiesAnalysis& operator=(const PropertiesAnalysis&) = delete;

      void analyse(
          const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem) noexcept;

      double getPlausibility() const noexcept;

    protected:
      double plausibility_;

      virtual void analyseImplementation(
          const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem) noexcept = 0;
  };

  template <typename ParameterType, class DistanceFunction>
  void PropertiesAnalysis<ParameterType, DistanceFunction>::analyse(
      const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem) noexcept {
    plausibility_ = 0.0;

    analyseImplementation(optimisationProblem);
  }

  template <typename ParameterType, class DistanceFunction>
  double PropertiesAnalysis<ParameterType, DistanceFunction>::getPlausibility() const noexcept {
    return plausibility_;
  }
}
