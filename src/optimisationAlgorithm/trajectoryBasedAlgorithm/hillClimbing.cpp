#include <hop_bits/optimisationAlgorithm/trajectoryBasedAlgorithm/hillClimbing.hpp>

// C++ Standard Library
#include <limits>

// HOP
#include <hop_bits/helper/rng.hpp>

namespace hop {
  HillClimbing::HillClimbing(
      const std::shared_ptr<OptimisationProblem> optimisationProblem)
    : TrajectoryBasedAlgorithm(optimisationProblem),
      candidateObjectiveValue_(std::numeric_limits<double>::infinity()),
      candidateSoftConstraintValue_(std::numeric_limits<double>::infinity()) {
    setMaximalStepSize((optimisationProblem->getUpperBounds() - optimisationProblem->getLowerBounds()) / 10);
  }

  void HillClimbing::optimiseImplementation() {
    ++numberOfIterations_;

    bestParameter_ = initialParameter_;
    bestSoftConstraintValue_ = optimisationProblem_->getSoftConstraintsValue(initialParameter_);
    bestObjectiveValue_ = optimisationProblem_->getObjectiveValue(initialParameter_);

    while(!isFinished() && !isTerminated()) {
      ++numberOfIterations_;
      //std::cout << maximalStepSize_ << std::endl;
      candidateParameter_ = bestParameter_ + maximalStepSize_ % getVelocity();
      candidateSoftConstraintValue_ = optimisationProblem_->getSoftConstraintsValue(candidateParameter_);
      candidateObjectiveValue_ = optimisationProblem_->getObjectiveValue(candidateParameter_);

      if(candidateSoftConstraintValue_ < bestSoftConstraintValue_ || candidateSoftConstraintValue_ == bestSoftConstraintValue_ && candidateObjectiveValue_ < bestObjectiveValue_) {
        bestParameter_ = candidateParameter_;
        bestSoftConstraintValue_ = candidateSoftConstraintValue_;
        bestObjectiveValue_ = candidateObjectiveValue_;
      }
    }
  }

  arma::Col<double> HillClimbing::getVelocity() {
    return arma::normalise(arma::randn<arma::Col<double>>(optimisationProblem_->getNumberOfDimensions())) * std::uniform_real_distribution<double>(0, 1)(Rng::generator);
  }

  void HillClimbing::setMaximalStepSize(
      const arma::Col<double>& maximalStepSize) {
    if(maximalStepSize.n_rows != optimisationProblem_->getNumberOfDimensions()) {
      throw std::logic_error("The dimension of the maximal step size (" + std::to_string(maximalStepSize.n_elem) + ") must match the dimension of the optimisation problem (" + std::to_string(optimisationProblem_->getNumberOfDimensions()) + ").");
    }

    maximalStepSize_ = maximalStepSize;
  }

  std::string HillClimbing::to_string() const noexcept {
    return "HillClimbing";
  }
}
