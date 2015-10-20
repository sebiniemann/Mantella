#include "mantella_bits/optimisationAlgorithm/trajectoryBasedOptimisationAlgorithm.hpp"

// Mantella
#include "mantella_bits/helper/assert.hpp"

namespace mant {
  TrajectoryBasedOptimisationAlgorithm::TrajectoryBasedOptimisationAlgorithm(
      const std::shared_ptr<OptimisationProblem> optimisationProblem)
      : OptimisationAlgorithm(optimisationProblem) {
    setInitialParameter(arma::randu<arma::Col<double>>(numberOfDimensions_) % (getUpperBounds() - getLowerBounds()) + getLowerBounds());
  }

  void TrajectoryBasedOptimisationAlgorithm::setInitialParameter(
      const arma::Col<double>& initialParameter) {
    verify(initialParameter.n_elem == numberOfDimensions_, "The number of dimensions of the initial parameter must match the number of dimensions of the optimisation problem.");

    initialParameter_ = initialParameter;
  }
}
