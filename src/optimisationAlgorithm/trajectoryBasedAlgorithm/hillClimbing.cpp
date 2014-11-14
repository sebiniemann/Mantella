#include <hop_bits/optimisationAlgorithm/trajectoryBasedAlgorithm/hillClimbing.hpp>

// HOP
#include <hop_bits/helper/rng.hpp>

// TODO setInitialParameter
namespace hop {
  HillClimbing::HillClimbing(
      const std::shared_ptr<OptimisationProblem> optimisationProblem)
    : TrajectoryBasedAlgorithm(optimisationProblem) {
    setMaximalStepSize((optimisationProblem->getUpperBounds() - optimisationProblem->getLowerBounds()) / 10);
  }

  void HillClimbing::optimiseImplementation() {
    bestParameter_ = initialParameter_;
    bestObjectiveValue_ = optimisationProblem_->getObjectiveValue(initialParameter_);
    bestSoftConstraintValue_ = optimisationProblem_->getSoftConstraintsValue(initialParameter_);

    while(!isFinished() && !isTerminated()) {
      ++numberOfIterations_;

      arma::Col<double> parameter = bestParameter_ + maximalStepSize_ % arma::normalise(arma::randn<arma::Col<double>>(optimisationProblem_->getNumberOfDimensions())) * std::uniform_real_distribution<double>(0, 1)(Rng::generator);

      double objectiveValue = optimisationProblem_->getObjectiveValue(parameter);
      double softConstraintValue = optimisationProblem_->getSoftConstraintsValue(parameter);

      if(softConstraintValue < bestSoftConstraintValue_ || softConstraintValue == bestSoftConstraintValue_ && objectiveValue < bestObjectiveValue_) {
        bestParameter_ = parameter;
        bestObjectiveValue_ = softConstraintValue;
        bestSoftConstraintValue_ = objectiveValue;
      }
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
