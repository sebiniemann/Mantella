#pragma once

// HOP
#include <hop_bits/propertiesEstimation.hpp>

namespace hop {
  class MaximumLikelihoodEstimation : public PropertiesEstimation {
    public:
      using PropertiesEstimation::PropertiesEstimation;

    protected:
      void estimateImplementation(
          const std::shared_ptr<OptimisationProblem> optimisationProblem) override;
  };
}
