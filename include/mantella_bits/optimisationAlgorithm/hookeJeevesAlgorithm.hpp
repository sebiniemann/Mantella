#pragma once

// Mantella
#include "mantella_bits/optimisationAlgorithm.hpp"
namespace mant {
  class OptimisationProblem;
}

namespace mant {
  class HookeJeevesAlgorithm : public OptimisationAlgorithm {
   public:
    explicit HookeJeevesAlgorithm();

    using OptimisationAlgorithm::optimise;

    void optimise(
        OptimisationProblem& optimisationProblem);

    void setInitialStepSize(
        const double initialStepSize);
    double getInitialStepSize() const;

    void setStepSizeDecrease(
        const double stepSizeDecrease);
    double getStepSizeDecrease() const;

    // Public accessible to be used in lambdas
    double stepSize_;

   protected:
    double initialStepSize_;
    double stepSizeDecrease_;
  };
}
