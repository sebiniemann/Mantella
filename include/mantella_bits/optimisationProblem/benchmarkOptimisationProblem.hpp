#pragma once

// Mantella
#include "mantella_bits/optimisationProblem.hpp"

namespace mant {
  class BenchmarkOptimisationProblem : public OptimisationProblem {
   public:
    using OptimisationProblem::OptimisationProblem;

    virtual double getOptimalObjectiveValue() const = 0;

    virtual ~BenchmarkOptimisationProblem() = default;
  };
}
