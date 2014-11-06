#pragma once

// C++ Standard Library
#include <memory>

// Armadillo
#include <armadillo>

// HOP
#include <hop_bits/helper/cache.hpp>
#include <hop_bits/optimisationProblem.hpp>

namespace hop {
  class PropertiesAnalysis {
    public:
      explicit PropertiesAnalysis() = default;

      // Copy constructors are not used in this library and deleted to avoid unintended/any usage.
      PropertiesAnalysis(const PropertiesAnalysis&) = delete;
      PropertiesAnalysis& operator=(const PropertiesAnalysis&) = delete;

      void analyse(
          const std::shared_ptr<OptimisationProblem> optimisationProblem);

      double getBelief() const;
      double getPlausibility() const;

    protected:
      double belief_;
      double plausibility_;

      virtual void analyseImplementation(
          const arma::Mat<double>& parameters,
          const arma::Col<double>& objectiveValues) = 0;
  };
}
