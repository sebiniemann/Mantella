#pragma once

// HOP
#include <hop_bits/optimisationAlgorithm.hpp>

namespace hop {
  class ParallelOptimisationAlgorithm : public OptimisationAlgorithm {
    public:
      explicit ParallelOptimisationAlgorithm(const std::shared_ptr<OptimisationProblem> optimisationProblem);

      int getRank() const;
      int getNumberOfNodes() const;

    protected:
      int rank_;
      int numberOfNodes_;

      void optimiseImplementation() final override;

      virtual void parallelOptimiseImplementation() = 0;
  };
}
