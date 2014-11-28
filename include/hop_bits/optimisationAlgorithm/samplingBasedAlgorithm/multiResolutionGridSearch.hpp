#pragma once

// HOP
#include <hop_bits/optimisationAlgorithm/samplingBasedAlgorithm.hpp>

namespace hop {
  class MultiResolutionGridSearch : public SamplingBasedAlgorithm {
    public:
      explicit MultiResolutionGridSearch(
          const std::shared_ptr<OptimisationProblem> optimisationProblem) noexcept;

      MultiResolutionGridSearch(const MultiResolutionGridSearch&) = delete;
      MultiResolutionGridSearch& operator=(const MultiResolutionGridSearch&) = delete;

      void setAverageSamplesPerDimension(
          const unsigned int& averageSamplesPerDimension) noexcept;

      void setSamplingFactors(
          const arma::Col<double>& samplingFactors);

      std::string to_string() const noexcept override;

    protected:
      unsigned int averageSamplesPerDimension_;
      arma::Col<double> samplingFactors_;

      void optimiseImplementation() noexcept override;
  };
}
