#pragma once

// HOP
#include <hop_bits/optimisationAlgorithm/samplingBasedAlgorithm.hpp>

namespace hop {
  class GridSearch : public SamplingBasedAlgorithm {
    public:
      explicit GridSearch(
          const std::shared_ptr<OptimisationProblem> optimisationProblem) noexcept;

      GridSearch(const GridSearch&) = delete;
      GridSearch& operator=(const GridSearch&) = delete;

      void setSamplingFactors(
          const arma::Col<double>& samplingFactors);

      std::string to_string() const noexcept override;

    protected:
      arma::Col<double> samplingFactors_;

      void optimiseImplementation() noexcept override;
  };
}
