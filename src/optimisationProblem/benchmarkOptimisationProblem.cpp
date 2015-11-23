#include "mantella_bits/optimisationProblem/benchmarkOptimisationProblem.hpp"

// Mantella
#include "mantella_bits/assert.hpp"

namespace mant {
  void BenchmarkOptimisationProblem::setAcceptableObjectiveValuePrecision(
      const double acceptableObjectiveValuePrecision) {
    verify(acceptableObjectiveValuePrecision >= 0, "setAcceptableObjectiveValuePrecision: The acceptable objective value precision must be positive (including zero).");

    acceptableObjectiveValuePrecision_ = acceptableObjectiveValuePrecision;
#if defined(SUPPORT_MPI)
    MPI_Bcast(&acceptableObjectiveValuePrecision_, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
#endif

    // The *acceptableObjectiveValueThreshold_* is used by all optimisation algorithm, and should be set relative to *objectiveValueTranslation_*, assuming that all benchmark problem have 0 as their optimal (reachable) objective value in the default (unmodified) case.
    setAcceptableObjectiveValueThreshold(objectiveValueTranslation_ + acceptableObjectiveValuePrecision_);
  }

  double BenchmarkOptimisationProblem::getAcceptableObjectiveValuePrecision() const {
    return acceptableObjectiveValuePrecision_;
  }
}
