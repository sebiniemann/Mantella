#include <hop_bits/optimisationAlgorithm/trajectoryBasedAlgorithm.hpp>

namespace hop {
  TrajectoryBasedAlgorithm::TrajectoryBasedAlgorithm(
      const std::shared_ptr<OptimisationProblem> optimisationProblem)
    : OptimisationAlgorithm(optimisationProblem) {
    setInitialParameter(arma::randu<arma::Col<double>>(optimisationProblem_->getNumberOfDimensions()) % (optimisationProblem_->getUpperBounds() - optimisationProblem_->getLowerBounds()) + optimisationProblem_->getLowerBounds());
  }

  void TrajectoryBasedAlgorithm::setInitialParameter(
      const arma::Col<double>& initialParameter) {
    initialParameter_ = initialParameter;
  }
}
