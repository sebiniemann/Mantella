#include <hop_bits/optimisationAlgorithm/trajectoryBasedAlgorithm.hpp>

namespace hop {
  TrajectoryBasedAlgorithm::TrajectoryBasedAlgorithm(
      const std::shared_ptr<OptimisationProblem> optimisationProblem) noexcept
    : OptimisationAlgorithm(optimisationProblem) {
    setInitialParameter(arma::randu<arma::Col<double>>(optimisationProblem_->getNumberOfDimensions()) % (optimisationProblem_->getUpperBounds() - optimisationProblem_->getLowerBounds()) + optimisationProblem_->getLowerBounds());
  }

  void TrajectoryBasedAlgorithm::setInitialParameter(
      const arma::Col<double>& initialParameter) {
    if(initialParameter.n_elem != optimisationProblem_->getNumberOfDimensions()) {
      throw std::logic_error("The number of dimensions of the initial parameter (" + std::to_string(initialParameter.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(optimisationProblem_->getNumberOfDimensions()) + ").");
    }

    initialParameter_ = initialParameter;
  }
}
