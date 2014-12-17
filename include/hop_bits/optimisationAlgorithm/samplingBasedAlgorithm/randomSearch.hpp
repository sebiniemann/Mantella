#pragma once

// HOP
#include <hop_bits/optimisationAlgorithm/samplingBasedAlgorithm.hpp>

namespace hop {
  class RandomSearch : public SamplingBasedAlgorithm<double> {
    public:
      explicit RandomSearch(
          const std::shared_ptr<OptimisationProblem<double>> optimisationProblem) noexcept;

      RandomSearch(const RandomSearch&) = delete;
      RandomSearch& operator=(const RandomSearch&) = delete;

      std::string to_string() const noexcept override;

    protected:
      void optimiseImplementation() noexcept override;
  };
}
