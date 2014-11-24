#pragma once

// C++ Standard Library
#include <vector>

// HOP
#include <hop_bits/optimisationAlgorithm/samplingBasedAlgorithm.hpp>

namespace hop {
  class GridSearch : public SamplingBasedAlgorithm {
    public:
      GridSearch(
          const std::shared_ptr<OptimisationProblem> optimisationProblem) noexcept;

      GridSearch(const GridSearch&) = delete;
      GridSearch& operator=(const GridSearch&) = delete;

      void setSamplingFactors(
          const arma::Col<double>& samplingFactors);

      std::string to_string() const noexcept override;

    protected:
      arma::Col<double> samplingFactors_;

      arma::Col<arma::uword> numberOfSamples_;
      std::vector<arma::Col<double>> sampleParameters_;
      arma::Col<arma::uword> sampleIndicies_;

      arma::Col<double> candidateParameter_;
      double candidateSoftConstraintValue_;
      double candidateObjectiveValue_;

      void optimiseImplementation() noexcept override;
  };
}
