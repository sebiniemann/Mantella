#pragma once

// Mantella
#include "mantella_bits/optimisationAlgorithm.hpp"
namespace mant {
  class OptimisationProblem;
}

namespace mant {
  class HillClimbing : public OptimisationAlgorithm {
   public:
    explicit HillClimbing();

    using OptimisationAlgorithm::optimise;

    void optimise(
        OptimisationProblem& optimisationProblem);

    void setMinimalStepSize(
        const double minimalStepSize);
    double getMinimalStepSize() const;

    void setMaximalStepSize(
        const double maximalStepSize);
    double getMaximalStepSize() const;

   protected:
    double minimalStepSize_;
    double maximalStepSize_;
  };
}
