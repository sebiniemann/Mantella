#pragma once

// C++ Standard Library
#include <memory>

// Mantella
#include <mantella_bits/optimisationProblem.hpp>

namespace mant {
  template <typename ParameterType, class DistanceFunction>
  class PropertiesAnalysis {
    public:
      explicit PropertiesAnalysis() noexcept;

      // Copy constructors are not used in this library and deleted to avoid unintended/any usage.
      PropertiesAnalysis(const PropertiesAnalysis&) = delete;
      PropertiesAnalysis& operator=(const PropertiesAnalysis&) = delete;

      void analyse(
          const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem) noexcept;

      double getPlausibility() const noexcept;

    protected:
      double plausibility_;

      DistanceFunction distanceFunction_;

      virtual void analyseImplementation(
          const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem) noexcept = 0;
  };

  template <typename ParameterType, class DistanceFunction>
  PropertiesAnalysis<ParameterType, DistanceFunction>::PropertiesAnalysis() noexcept
    : plausibility_(0.0),
      distanceFunction_(DistanceFunction()) {

  }

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
