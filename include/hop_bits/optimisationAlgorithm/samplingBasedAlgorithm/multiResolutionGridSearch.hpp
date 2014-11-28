#pragma once

// C++ Standard Library
#include <vector>

// HOP
#include <hop_bits/optimisationAlgorithm/samplingBasedAlgorithm.hpp>

namespace hop {
  class MultiResolutionGridSearch : public SamplingBasedAlgorithm {
    public:
      explicit MultiResolutionGridSearch(
          const std::shared_ptr<OptimisationProblem> optimisationProblem) noexcept;

      MultiResolutionGridSearch(const MultiResolutionGridSearch&) = delete;
      MultiResolutionGridSearch& operator=(const MultiResolutionGridSearch&) = delete;

      void setMaximalResolutionDepth(
          const unsigned int& maximalResolutionDepth) noexcept;

      void setSamplingFactors(
          const arma::Col<double>& samplingFactors);

      std::string to_string() const noexcept override;

    protected:
      unsigned int maximalResolutionDepth_;
      arma::Col<double> samplingFactors_;

      arma::Col<arma::uword> numberOfSamples_;
      std::vector<arma::Col<double>> sampleParameters_;
      arma::Col<arma::uword> sampleIndicies_;

      arma::Col<double> candidateParameter_;
      double candidateSoftConstraintsValue_;
      double candidateObjectiveValue_;

      void optimiseImplementation() noexcept override;
  };
}
