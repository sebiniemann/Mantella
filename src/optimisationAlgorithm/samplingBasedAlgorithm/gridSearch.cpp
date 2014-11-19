#include <hop_bits/optimisationAlgorithm/samplingBasedAlgorithm/gridSearch.hpp>

namespace hop {
  GridSearch::GridSearch(
      const std::shared_ptr<OptimisationProblem> optimisationProblem)
    : SamplingBasedAlgorithm(optimisationProblem),
      candidateObjectiveValue_(std::numeric_limits<double>::infinity()),
      candidateSoftConstraintValue_(std::numeric_limits<double>::infinity()) {

  }

  void GridSearch::optimiseImplementation() {
    bestSoftConstraintValue_ = std::numeric_limits<double>::infinity();
    bestObjectiveValue_ = std::numeric_limits<double>::infinity();

    while(!isFinished() && !isTerminated()) {
      ++numberOfIterations_;



      if(candidateSoftConstraintValue_ < bestSoftConstraintValue_ || candidateSoftConstraintValue_ == bestSoftConstraintValue_ && candidateObjectiveValue_ < bestObjectiveValue_) {
        bestParameter_ = candidateParameter_;
        bestSoftConstraintValue_ = candidateSoftConstraintValue_;
        bestObjectiveValue_ = candidateObjectiveValue_;
      }
    }
  }

  std::string GridSearch::to_string() const noexcept {
    return "GridSearch";
  }
}
