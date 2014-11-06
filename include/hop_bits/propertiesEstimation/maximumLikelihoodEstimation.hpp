#pragma once

// HOP
#include <hop_bits/propertiesEstimation.hpp>

namespace hop {
  class MaximumLikelihoodEstimation : public PropertiesEstimation {
    public:
      void estimate(
          const std::shared_ptr<OptimisationProblem>& optimisationProblem) const override;
  };
}
