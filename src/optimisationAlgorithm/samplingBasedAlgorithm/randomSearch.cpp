#include <hop_bits/optimisationAlgorithm/samplingBasedAlgorithm/randomSearch.hpp>

namespace hop {
  RandomSearch::RandomSearch(
      const std::shared_ptr<OptimisationProblem> optimisationProblem) noexcept
    : SamplingBasedAlgorithm(optimisationProblem),
      candidateObjectiveValue_(std::numeric_limits<double>::infinity()),
      candidateSoftConstraintValue_(std::numeric_limits<double>::infinity()) {

  }

  void RandomSearch::optimiseImplementation() noexcept {
    bestSoftConstraintValue_ = std::numeric_limits<double>::infinity();
    bestObjectiveValue_ = std::numeric_limits<double>::infinity();

    do {
      ++numberOfIterations_;

      candidateParameter_ = arma::randu<arma::Col<double>>(optimisationProblem_->getNumberOfDimensions()) % (optimisationProblem_->getUpperBounds() - optimisationProblem_->getLowerBounds()) + optimisationProblem_->getLowerBounds();
      candidateSoftConstraintValue_ = optimisationProblem_->getSoftConstraintsValue(candidateParameter_);
      candidateObjectiveValue_ = optimisationProblem_->getObjectiveValue(candidateParameter_);

      if(candidateSoftConstraintValue_ < bestSoftConstraintValue_ || candidateSoftConstraintValue_ == bestSoftConstraintValue_ && candidateObjectiveValue_ < bestObjectiveValue_) {
        bestParameter_ = candidateParameter_;
        bestSoftConstraintValue_ = candidateSoftConstraintValue_;
        bestObjectiveValue_ = candidateObjectiveValue_;
      }
    } while(!isFinished() && !isTerminated());
  }

  std::string RandomSearch::to_string() const noexcept {
    return "PureRandomSearch";
  }
}
