#pragma once

#include <hop_bits/optimisationAlgorithm.hpp>

namespace hop {
  class RandomSearch : public OptimisationAlgorithm {
    public:
      RandomSearch(const std::shared_ptr<OptimisationProblem> optimisationProblem);

      RandomSearch(const RandomSearch&) = delete;
      RandomSearch& operator=(const RandomSearch&) = delete;

    protected:
      bool optimiseImplementation() override;
  };
}
