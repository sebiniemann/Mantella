#pragma once

// HOP
#include <hop_bits/optimisationAlgorithm/samplingBasedAlgorithm.hpp>

namespace hop {
  class GridSearch : public SamplingBasedAlgorithm {
    public:
      GridSearch(
          const std::shared_ptr<OptimisationProblem> optimisationProblem);

      GridSearch(const GridSearch&) = delete;
      GridSearch& operator=(const GridSearch&) = delete;

      std::string to_string() const noexcept override;

    protected:
      arma::Col<double> candidateParameter_;
      double candidateSoftConstraintValue_;
      double candidateObjectiveValue_;

      void optimiseImplementation() override;
  };
}
