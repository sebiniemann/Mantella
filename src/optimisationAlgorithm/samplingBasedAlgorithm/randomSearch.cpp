#include <hop_bits/optimisationAlgorithm/samplingBasedAlgorithm/randomSearch.hpp>

namespace hop {
  RandomSearch::RandomSearch(
      const std::shared_ptr<OptimisationProblem> optimisationProblem) noexcept
    : SamplingBasedAlgorithm(optimisationProblem) {

  }

  void RandomSearch::optimiseImplementation() noexcept {
    bestSoftConstraintsValue_ = std::numeric_limits<double>::infinity();
    bestObjectiveValue_ = std::numeric_limits<double>::infinity();

    do {
      ++numberOfIterations_;

      const arma::Col<double>& candidateParameter = arma::randu<arma::Col<double>>(optimisationProblem_->getNumberOfDimensions()) % (optimisationProblem_->getUpperBounds() - optimisationProblem_->getLowerBounds()) + optimisationProblem_->getLowerBounds();
      const double& candidateSoftConstraintsValue = optimisationProblem_->getSoftConstraintsValue(candidateParameter);
      const double& candidateObjectiveValue = optimisationProblem_->getObjectiveValue(candidateParameter);

      if(candidateSoftConstraintsValue < bestSoftConstraintsValue_ || candidateSoftConstraintsValue == bestSoftConstraintsValue_ && candidateObjectiveValue < bestObjectiveValue_) {
        bestParameter_ = candidateParameter;
        bestSoftConstraintsValue_ = candidateSoftConstraintsValue;
        bestObjectiveValue_ = candidateObjectiveValue;
      }
    } while(!isFinished() && !isTerminated());
  }

  std::string RandomSearch::to_string() const noexcept {
    return "RandomSearch";
  }
}
