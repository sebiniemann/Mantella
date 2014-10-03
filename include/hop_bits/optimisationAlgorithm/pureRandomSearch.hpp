#pragma once

#include <hop_bits/optimisationAlgorithm.hpp>

namespace hop {
  class PureRandomSearch : public OptimisationAlgorithm {
    public:
      PureRandomSearch(const std::shared_ptr<OptimisationProblem> optimisationProblem);

      PureRandomSearch(const PureRandomSearch&) = delete;
      PureRandomSearch& operator=(const PureRandomSearch&) = delete;

      std::string to_string() const override;

    protected:
      void optimiseImplementation() override;
  };
}
