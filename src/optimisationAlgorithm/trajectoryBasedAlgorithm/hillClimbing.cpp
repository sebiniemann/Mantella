#include <hop_bits/optimisationAlgorithm/trajectoryBasedAlgorithm/hillClimbing.hpp>

// HOP
#include <hop_bits/helper/rng.hpp>

namespace hop {
  HillClimbing::HillClimbing(
      const std::shared_ptr<OptimisationProblem> optimisationProblem)
    : TrajectoryBasedAlgorithm(optimisationProblem) {
    setMaximalStepSize((optimisationProblem->getUpperBounds() - optimisationProblem->getLowerBounds()) / 10);
  }

  void HillClimbing::optimiseImplementation() {
    while(!isFinished() && !isTerminated()) {
      ++numberOfIterations_;
      updateBestParameter(bestParameter_ + maximalStepSize_ % arma::normalise(arma::randn<arma::Col<double>>(optimisationProblem_->getNumberOfDimensions())) * std::uniform_real_distribution<double>(0, 1)(Rng::generator));
    }
  }

  void HillClimbing::setMaximalStepSize(
      const arma::Col<double>& maximalStepSize) {
    maximalStepSize_ = maximalStepSize;
  }

  std::string HillClimbing::to_string() const {
    return "HillClimbing";
  }
}
