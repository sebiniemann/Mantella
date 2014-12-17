#pragma once

// HOP
#include <hop_bits/propertiesEstimation.hpp>

namespace hop {
  class MaximumLikelihoodEstimation : public PropertiesEstimation<double> {
    public:
      using PropertiesEstimation<double>::PropertiesEstimation;

    protected:
      void estimateImplementation(
          const std::shared_ptr<OptimisationProblem<double>> optimisationProblem) noexcept override;
  };
}
