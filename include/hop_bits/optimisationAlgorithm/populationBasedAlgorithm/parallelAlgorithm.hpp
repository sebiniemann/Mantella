#pragma once

// HOP
#include <hop_bits/optimisationAlgorithm/populationBasedAlgorithm.hpp>

namespace hop {
  class ParallelAlgorithm : public PopulationBasedAlgorithm {
    public:
      explicit ParallelAlgorithm(
          const std::shared_ptr<OptimisationProblem> optimisationProblem);

      int getRank() const;
      int getNumberOfNodes() const;

    protected:
      int rank_;
      int numberOfNodes_;

      void optimiseImplementation() final override;

      virtual void parallelOptimiseImplementation() = 0;
  };
}
