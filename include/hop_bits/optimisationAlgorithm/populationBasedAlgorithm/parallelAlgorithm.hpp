#pragma once

// HOP
#include <hop_bits/optimisationAlgorithm/populationBasedAlgorithm.hpp>

namespace hop {
  class ParallelAlgorithm : public PopulationBasedAlgorithm {
    public:
      explicit ParallelAlgorithm(
          const std::shared_ptr<OptimisationProblem> optimisationProblem,
          const unsigned int& populationSize) noexcept;

      unsigned int getRank() const noexcept;
      unsigned int getNumberOfNodes() const noexcept;

    protected:
      int rank_;
      int numberOfNodes_;

      void optimiseImplementation() noexcept final override;

      virtual void parallelOptimiseImplementation() noexcept = 0;
  };
}
