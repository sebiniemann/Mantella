#include <hop_bits/optimisationAlgorithm/simulatedAnnealing.hpp>

#include <random>
#include <cmath>

#include <hop_bits/helper/random.hpp>

namespace hop {
  SimulatedAnnealing::SimulatedAnnealing(const std::shared_ptr<OptimisationProblem> optimisationProblem)
    : OptimisationAlgorithm(optimisationProblem) {
    setMaximalStepSize((optimisationProblem->getUpperBounds() - optimisationProblem->getLowerBounds()) / 10);
  }

  void SimulatedAnnealing::optimiseImplementation() {
    arma::Col<double> candidateSolution;
    do {
      candidateSolution = arma::randu<arma::Col<double>>(optimisationProblem_->getNumberOfDimensions()) % (optimisationProblem_->getUpperBounds() - optimisationProblem_->getLowerBounds()) + optimisationProblem_->getLowerBounds();
    } while(!optimisationProblem_->isSatisfyingConstraints(candidateSolution));

    ++numberOfIterations_;
    state_ = candidateSolution;
    objectiveValue_ = optimisationProblem_->getObjectiveValue(candidateSolution) + optimisationProblem_->getSoftConstraintsValue(candidateSolution);

    bestSolution_ = candidateSolution;
    bestObjectiveValue_ = objectiveValue_;

    while(!isFinished() && !isTerminated()) {
      ++numberOfIterations_;

      arma::Col<double> candidateSolution;
      do {
        candidateSolution = state_ + maximalStepSize_ % arma::normalise(arma::randn<arma::Col<double>>(optimisationProblem_->getNumberOfDimensions())) * std::uniform_real_distribution<double>(0, 1)(Random::Rng);
      } while(!optimisationProblem_->isSatisfyingConstraints(candidateSolution));

      double objectiveValue = optimisationProblem_->getObjectiveValue(candidateSolution) + optimisationProblem_->getSoftConstraintsValue(candidateSolution);

      if (objectiveValue < objectiveValue_ || std::exp((objectiveValue_ - objectiveValue) / (numberOfIterations_ / maximalNumberOfIterations_)) < std::uniform_real_distribution<double>(0, 1)(Random::Rng)) {
        state_ = candidateSolution;
        objectiveValue_ = objectiveValue;
      }

      if (objectiveValue < bestObjectiveValue_) {
        bestSolution_ = candidateSolution;
        bestObjectiveValue_ = objectiveValue;
      }
    }
  }

  void SimulatedAnnealing::setMaximalStepSize(const arma::Col<double>& maximalStepSize) {
    maximalStepSize_ = maximalStepSize;
  }

  std::string SimulatedAnnealing::to_string() const {
    return "SimulatedAnnealing";
  }
}
