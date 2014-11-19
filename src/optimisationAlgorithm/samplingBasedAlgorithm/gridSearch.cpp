#include <hop_bits/optimisationAlgorithm/samplingBasedAlgorithm/gridSearch.hpp>

namespace hop {
  GridSearch::GridSearch(
      const std::shared_ptr<OptimisationProblem> optimisationProblem)
    : SamplingBasedAlgorithm(optimisationProblem),
      candidateObjectiveValue_(std::numeric_limits<double>::infinity()),
      candidateSoftConstraintValue_(std::numeric_limits<double>::infinity()) {

  }

  void GridSearch::optimiseImplementation() {
    numberOfSamples_ = arma::conv_to<arma::Col<arma::uword>>::from(arma::ceil(maximalNumberOfIterations_ * (optimisationProblem_->getUpperBounds() - optimisationProblem_->getLowerBounds()) / arma::sum(optimisationProblem_->getUpperBounds() - optimisationProblem_->getLowerBounds())));

    sampleParameters_.clear();
    for (std::size_t n = 0; n < optimisationProblem_->getNumberOfDimensions(); ++n) {
      sampleParameters_.push_back(arma::linspace(optimisationProblem_->getLowerBounds().at(n), optimisationProblem_->getUpperBounds().at(n), numberOfSamples_.at(n)));
    }

    bestSoftConstraintValue_ = std::numeric_limits<double>::infinity();
    bestObjectiveValue_ = std::numeric_limits<double>::infinity();

    sampleIndicies_ = arma::zeros<arma::Col<arma::uword>>(sampleParameters_.size());
    do {
      ++numberOfIterations_;

      for(std::size_t n = 0; n < sampleIndicies_.n_elem; ++n) {
        candidateParameter_.at(n) = sampleParameters_.at(n).at(sampleIndicies_.at(n));
      }

      ++sampleIndicies_.at(0);
      for(std::size_t n = 0; n < sampleIndicies_.n_elem - 1; ++n) {
        if(sampleIndicies_.at(n) >= numberOfSamples_.at(n)) {
           ++sampleIndicies_.at(n + 1);
        }
      }

      candidateParameter_ = candidateParameter_;
      candidateSoftConstraintValue_ = optimisationProblem_->getSoftConstraintsValue(candidateParameter_);
      candidateObjectiveValue_ = optimisationProblem_->getObjectiveValue(candidateParameter_);

      if(candidateSoftConstraintValue_ < bestSoftConstraintValue_ || candidateSoftConstraintValue_ == bestSoftConstraintValue_ && candidateObjectiveValue_ < bestObjectiveValue_) {
        bestParameter_ = candidateParameter_;
        bestSoftConstraintValue_ = candidateSoftConstraintValue_;
        bestObjectiveValue_ = candidateObjectiveValue_;
      }
    } while(!isFinished() && !isTerminated());
  }

  std::string GridSearch::to_string() const noexcept {
    return "GridSearch";
  }
}
