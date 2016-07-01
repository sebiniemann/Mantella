#pragma once

// Mantella
#include "mantella_bits/optimisationAlgorithm.hpp"
namespace mant {
  class OptimisationProblem;
}

namespace mant {
  class RandomSearch : public OptimisationAlgorithm {
   public:
    explicit RandomSearch();

    using OptimisationAlgorithm::optimise;

    void optimise(
        OptimisationProblem& optimisationProblem);
  };
}
