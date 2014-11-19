#pragma once

// HOP
#include <hop_bits/optimisationAlgorithm/samplingBasedAlgorithm.hpp>

namespace hop {
  class PureRandomSearch : public SamplingBasedAlgorithm {
    public:
      PureRandomSearch(
          const std::shared_ptr<OptimisationProblem> optimisationProblem);

      PureRandomSearch(const PureRandomSearch&) = delete;
      PureRandomSearch& operator=(const PureRandomSearch&) = delete;

      std::string to_string() const noexcept override;

    protected:
      arma::Col<double> candidateParameter_;
      double candidateSoftConstraintValue_;
      double candidateObjectiveValue_;

      void optimiseImplementation() override;
  };
}
