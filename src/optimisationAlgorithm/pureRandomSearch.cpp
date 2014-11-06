#include <hop_bits/optimisationAlgorithm/pureRandomSearch.hpp>

namespace hop {
  PureRandomSearch::PureRandomSearch(
      const std::shared_ptr<OptimisationProblem> optimisationProblem)
    : OptimisationAlgorithm(optimisationProblem) {

  }

  void PureRandomSearch::optimiseImplementation() {
    while(!isFinished() && !isTerminated()) {
      ++numberOfIterations_;

      arma::Col<double> candidateSolution;
      do {
        candidateSolution = arma::randu<arma::Col<double>>(optimisationProblem_->getNumberOfDimensions()) % (optimisationProblem_->getUpperBounds() - optimisationProblem_->getLowerBounds()) + optimisationProblem_->getLowerBounds();
      } while(!optimisationProblem_->isSatisfyingConstraints(candidateSolution));

      double objectiveValue = optimisationProblem_->getObjectiveValue(candidateSolution) + optimisationProblem_->getSoftConstraintsValue(candidateSolution);

      if (objectiveValue < bestObjectiveValue_) {
        bestSolution_ = candidateSolution;
        bestObjectiveValue_ = objectiveValue;
      }
    }
  }

  std::string PureRandomSearch::to_string() const {
    return "PureRandomSearch";
  }
}
