#include <hop_bits/optimisationAlgorithm/randomSearch.hpp>

namespace hop {
  RandomSearch::RandomSearch(const std::shared_ptr<OptimisationProblem> optimisationProblem)
    : OptimisationAlgorithm(optimisationProblem) {

  }

  void RandomSearch::optimiseImplementation() {
    while(true) {
      ++numberOfIterations_;

      arma::Col<double> candidateSolution;
      do {
        candidateSolution = arma::randu<arma::Col<double>>(optimisationProblem_->getNumberOfDimensions()) % (optimisationProblem_->getUpperBounds() - optimisationProblem_->getLowerBounds()) + optimisationProblem_->getLowerBounds();
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
}
