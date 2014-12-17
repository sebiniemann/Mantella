#include <hop_bits/optimisationAlgorithm/trajectoryBasedAlgorithm/hillClimbing.hpp>

// C++ Standard Library
#include <limits>

// HOP
#include <hop_bits/helper/rng.hpp>

// TODO Add restarting
namespace hop {
  HillClimbing::HillClimbing(
      const std::shared_ptr<OptimisationProblem<double>> optimisationProblem) noexcept
    : TrajectoryBasedAlgorithm(optimisationProblem) {
    setMaximalStepSize((optimisationProblem->getUpperBounds() - optimisationProblem->getLowerBounds()) / 10.0);
  }

  void HillClimbing::optimiseImplementation() noexcept {
    ++numberOfIterations_;

    bestParameter_ = initialParameter_;
    bestSoftConstraintsValue_ = optimisationProblem_->getSoftConstraintsValue(initialParameter_);
    bestObjectiveValue_ = optimisationProblem_->getObjectiveValue(initialParameter_);

    while(!isFinished() && !isTerminated()) {
      ++numberOfIterations_;

      arma::Col<double> candidateParameter = bestParameter_ + maximalStepSize_ % getVelocity();

      const arma::Col<arma::uword>& belowLowerBound = arma::find(candidateParameter < optimisationProblem_->getLowerBounds());
      const arma::Col<arma::uword>& aboveUpperBound = arma::find(candidateParameter > optimisationProblem_->getUpperBounds());

      candidateParameter.elem(belowLowerBound) = optimisationProblem_->getLowerBounds().elem(belowLowerBound);
      candidateParameter.elem(aboveUpperBound) = optimisationProblem_->getUpperBounds().elem(aboveUpperBound);

      const double& candidateSoftConstraintsValue = optimisationProblem_->getSoftConstraintsValue(candidateParameter);
      const double& candidateObjectiveValue = optimisationProblem_->getObjectiveValue(candidateParameter);

      if(candidateSoftConstraintsValue < bestSoftConstraintsValue_ || (candidateSoftConstraintsValue == bestSoftConstraintsValue_ && candidateObjectiveValue < bestObjectiveValue_)) {
        bestParameter_ = candidateParameter;
        bestSoftConstraintsValue_ = candidateSoftConstraintsValue;
        bestObjectiveValue_ = candidateObjectiveValue;
      }
    }
  }

  arma::Col<double> HillClimbing::getVelocity() noexcept {
    return arma::normalise(arma::randn<arma::Col<double>>(optimisationProblem_->getNumberOfDimensions())) * std::uniform_real_distribution<double>(0.0, 1.0)(Rng::generator);
  }

  void HillClimbing::setMaximalStepSize(
      const arma::Col<double>& maximalStepSize) {
    if(maximalStepSize.n_rows != optimisationProblem_->getNumberOfDimensions()) {
      throw std::logic_error("The number of dimensions of the maximal step size (" + std::to_string(maximalStepSize.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(optimisationProblem_->getNumberOfDimensions()) + ").");
    } else if (arma::any(maximalStepSize <= 0)) {
      throw std::logic_error("The maximal step size must be strict greater than 0.");
    }

    maximalStepSize_ = maximalStepSize;
  }

  std::string HillClimbing::to_string() const noexcept {
    return "HillClimbing";
  }
}
