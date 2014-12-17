#pragma once

// HOP
#include <hop_bits/optimisationAlgorithm/samplingBasedAlgorithm.hpp>

namespace hop {
  class MultiResolutionGridSearch : public SamplingBasedAlgorithm<double> {
    public:
      explicit MultiResolutionGridSearch(
          const std::shared_ptr<OptimisationProblem<double>> optimisationProblem) noexcept;

      MultiResolutionGridSearch(const MultiResolutionGridSearch&) = delete;
      MultiResolutionGridSearch& operator=(const MultiResolutionGridSearch&) = delete;

      void setMinimalSamplingDistances(
          const arma::Col<double>& minimalSamplingDistances);

      void setMaximalSamplesPerResolution(
          const unsigned int& maximalSamplesPerResolution) noexcept;

      void setSamplingDistributionPerDimension(
          const arma::Col<double>& samplingDistributionPerDimension);

      std::string to_string() const noexcept override;

    protected:
      arma::Col<double> minimalSamplingDistances_;
      unsigned int maximalSamplesPerResolution_;
      arma::Col<double> samplingDistributionPerDimension_;

      void optimiseImplementation() noexcept override;
  };
}
