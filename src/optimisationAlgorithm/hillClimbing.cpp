#include <hop_bits/optimisationAlgorithm/hillClimbing.hpp>

#include <random>

#include <hop_bits/helper/random.hpp>

namespace hop {
  HillClimbing::HillClimbing(const std::shared_ptr<OptimisationProblem> optimisationProblem)
    : OptimisationAlgorithm(optimisationProblem) {
    setMaximalStepSize(arma::ones<arma::Col<double>>(optimisationProblem_->getNumberOfDimensions()));
  }

  void HillClimbing::optimiseImplementation() {
    arma::Col<double> candidateSolution;
    do {
      candidateSolution = arma::randu<arma::Col<double>>(optimisationProblem_->getNumberOfDimensions()) % (optimisationProblem_->getUpperBounds() - optimisationProblem_->getLowerBounds()) + optimisationProblem_->getLowerBounds();
    } while(!optimisationProblem_->isSatisfyingConstraints(candidateSolution));

    ++numberOfIterations_;
    bestObjectiveValue_ = optimisationProblem_->getObjectiveValue(candidateSolution) + optimisationProblem_->getSoftConstraintsValue(candidateSolution);
    bestSolution_ = candidateSolution;

    while(true) {
      ++numberOfIterations_;

      arma::Col<double> candidateSolution;
      do {
        candidateSolution = bestSolution_ + maximalStepSize_ % arma::normalise(arma::randn<arma::Col<double>>(optimisationProblem_->getNumberOfDimensions())) * std::uniform_real_distribution<double>(0, 1)(Random::Rng);
      } while(!optimisationProblem_->isSatisfyingConstraints(candidateSolution));

      double objectiveValue = optimisationProblem_->getObjectiveValue(candidateSolution) + optimisationProblem_->getSoftConstraintsValue(candidateSolution);

      if (objectiveValue < bestObjectiveValue_) {
        bestObjectiveValue_ = objectiveValue;
        bestSolution_ = candidateSolution;
      }

      if(isFinished() || isTerminated()) {
        return;
      }
    }
  }

  void HillClimbing::setMaximalStepSize(const arma::Col<double>& maximalStepSize) {
    maximalStepSize_ = maximalStepSize;
  }
}
