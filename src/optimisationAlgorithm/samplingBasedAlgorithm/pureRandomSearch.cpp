#include <hop_bits/optimisationAlgorithm/samplingBasedAlgorithm/pureRandomSearch.hpp>

namespace hop {
  PureRandomSearch::PureRandomSearch(
      const std::shared_ptr<OptimisationProblem> optimisationProblem)
    : SamplingBasedAlgorithm(optimisationProblem),
      candidateObjectiveValue_(std::numeric_limits<double>::infinity()),
      candidateSoftConstraintValue_(std::numeric_limits<double>::infinity()) {

  }

  void PureRandomSearch::optimiseImplementation() {
    bestSoftConstraintValue_ = std::numeric_limits<double>::infinity();
    bestObjectiveValue_ = std::numeric_limits<double>::infinity();

    while(!isFinished() && !isTerminated()) {
      ++numberOfIterations_;

      candidateParameter_ = arma::randu<arma::Col<double>>(optimisationProblem_->getNumberOfDimensions()) % (optimisationProblem_->getUpperBounds() - optimisationProblem_->getLowerBounds()) + optimisationProblem_->getLowerBounds();
      candidateSoftConstraintValue_ = optimisationProblem_->getSoftConstraintsValue(candidateParameter_);
      candidateObjectiveValue_ = optimisationProblem_->getObjectiveValue(candidateParameter_);

      if(candidateSoftConstraintValue_ < bestSoftConstraintValue_ || candidateSoftConstraintValue_ == bestSoftConstraintValue_ && candidateObjectiveValue_ < bestObjectiveValue_) {
        bestParameter_ = candidateParameter_;
        bestSoftConstraintValue_ = candidateSoftConstraintValue_;
        bestObjectiveValue_ = candidateObjectiveValue_;
      }
    }
  }

  std::string PureRandomSearch::to_string() const noexcept {
    return "PureRandomSearch";
  }
}
