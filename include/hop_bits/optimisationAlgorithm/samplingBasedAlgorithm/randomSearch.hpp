#pragma once

// HOP
#include <hop_bits/optimisationAlgorithm/samplingBasedAlgorithm.hpp>

namespace hop {
  class RandomSearch : public SamplingBasedAlgorithm {
    public:
      explicit RandomSearch(
          const std::shared_ptr<OptimisationProblem> optimisationProblem) noexcept;

      RandomSearch(const RandomSearch&) = delete;
      RandomSearch& operator=(const RandomSearch&) = delete;

      std::string to_string() const noexcept override;

    protected:
      arma::Col<double> candidateParameter_;
      double candidateSoftConstraintsValue_;
      double candidateObjectiveValue_;

      void optimiseImplementation() noexcept override;
  };
}
