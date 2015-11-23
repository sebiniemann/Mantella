#pragma once

// Mantella
#include "mantella_bits/optimisationProblem.hpp"

namespace mant {
  class BenchmarkOptimisationProblem : public OptimisationProblem {
   public:
    using OptimisationProblem::OptimisationProblem;

    void setAcceptableObjectiveValuePrecision(
        const double acceptableObjectiveValuePrecision);
    double getAcceptableObjectiveValuePrecision() const;

   protected:
    double acceptableObjectiveValuePrecision_;
  };
}
