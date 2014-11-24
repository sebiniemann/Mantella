#include <hop_bits/optimisationAlgorithm/samplingBasedAlgorithm/randomSearch.hpp>

namespace hop {
  RandomSearch::RandomSearch(
      const std::shared_ptr<OptimisationProblem> optimisationProblem) noexcept
    : SamplingBasedAlgorithm(optimisationProblem),
      candidateObjectiveValue_(std::numeric_limits<double>::infinity()),
      candidateSoftConstraintsValue_(std::numeric_limits<double>::infinity()) {

  }

  void RandomSearch::optimiseImplementation() noexcept {
    bestSoftConstraintsValue_ = std::numeric_limits<double>::infinity();
    bestObjectiveValue_ = std::numeric_limits<double>::infinity();

    do {
      ++numberOfIterations_;

      candidateParameter_ = arma::randu<arma::Col<double>>(optimisationProblem_->getNumberOfDimensions()) % (optimisationProblem_->getUpperBounds() - optimisationProblem_->getLowerBounds()) + optimisationProblem_->getLowerBounds();
      candidateSoftConstraintsValue_ = optimisationProblem_->getSoftConstraintsValue(candidateParameter_);
      candidateObjectiveValue_ = optimisationProblem_->getObjectiveValue(candidateParameter_);

      if(candidateSoftConstraintsValue_ < bestSoftConstraintsValue_ || candidateSoftConstraintsValue_ == bestSoftConstraintsValue_ && candidateObjectiveValue_ < bestObjectiveValue_) {
        bestParameter_ = candidateParameter_;
        bestSoftConstraintsValue_ = candidateSoftConstraintsValue_;
        bestObjectiveValue_ = candidateObjectiveValue_;
      }
    } while(!isFinished() && !isTerminated());
  }

  std::string RandomSearch::to_string() const noexcept {
    return "PureRandomSearch";
  }
}
