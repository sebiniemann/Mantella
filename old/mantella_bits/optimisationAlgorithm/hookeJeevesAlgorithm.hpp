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

   protected:
    double initialStepSize_;
    double stepSizeDecrease_;

    double stepSize_;
  };
}
