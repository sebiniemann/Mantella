#pragma once

// HOP
#include <hop_bits/optimisationAlgorithm/trajectoryBasedAlgorithm.hpp>

namespace hop {
  class PureRandomSearch : public TrajectoryBasedAlgorithm {
    public:
      PureRandomSearch(
          const std::shared_ptr<OptimisationProblem> optimisationProblem);

      PureRandomSearch(const PureRandomSearch&) = delete;
      PureRandomSearch& operator=(const PureRandomSearch&) = delete;

      std::string to_string() const override;

    protected:
      void optimiseImplementation() override;
  };
}
