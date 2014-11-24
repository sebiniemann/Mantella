#include <hop_bits/optimisationAlgorithm/samplingBasedAlgorithm/gridSearch.hpp>

#include <iostream>

namespace hop {
  GridSearch::GridSearch(
      const std::shared_ptr<OptimisationProblem> optimisationProblem) noexcept
    : SamplingBasedAlgorithm(optimisationProblem),
      candidateObjectiveValue_(std::numeric_limits<double>::infinity()),
      candidateSoftConstraintsValue_(std::numeric_limits<double>::infinity()) {
    setSamplingFactors(arma::ones(optimisationProblem_->getNumberOfDimensions()) / static_cast<double>(optimisationProblem_->getNumberOfDimensions()));
}

  void GridSearch::optimiseImplementation() noexcept {
    const arma::Col<double>& scaledSamplingFactors = samplingFactors_.at(0) / samplingFactors_;
    numberOfSamples_ = arma::conv_to<arma::Col<arma::uword>>::from(scaledSamplingFactors * std::pow(maximalNumberOfIterations_ / arma::prod(scaledSamplingFactors), 1.0 / static_cast<double>(optimisationProblem_->getNumberOfDimensions())));

    sampleParameters_.clear();
    for (std::size_t n = 0; n < optimisationProblem_->getNumberOfDimensions(); ++n) {
      sampleParameters_.push_back(arma::linspace(optimisationProblem_->getLowerBounds().at(n), optimisationProblem_->getUpperBounds().at(n), numberOfSamples_.at(n)));
    }

    bestSoftConstraintsValue_ = std::numeric_limits<double>::infinity();
    bestObjectiveValue_ = std::numeric_limits<double>::infinity();

    sampleIndicies_ = arma::zeros<arma::Col<arma::uword>>(sampleParameters_.size());
    candidateParameter_.set_size(optimisationProblem_->getNumberOfDimensions());
    do {
      ++numberOfIterations_;

      for(std::size_t n = 0; n < sampleIndicies_.n_elem; ++n) {
        candidateParameter_.at(n) = sampleParameters_.at(n).at(sampleIndicies_.at(n));
      }

      ++sampleIndicies_.at(0);
      for(std::size_t n = 0; n < sampleIndicies_.n_elem - 1; ++n) {
        if(sampleIndicies_.at(n) >= numberOfSamples_.at(n)) {
          sampleIndicies_.at(n) = 0;
           ++sampleIndicies_.at(n + 1);
        }
      }

      candidateSoftConstraintsValue_ = optimisationProblem_->getSoftConstraintsValue(candidateParameter_);
      candidateObjectiveValue_ = optimisationProblem_->getObjectiveValue(candidateParameter_);

      if(candidateSoftConstraintsValue_ < bestSoftConstraintsValue_ || candidateSoftConstraintsValue_ == bestSoftConstraintsValue_ && candidateObjectiveValue_ < bestObjectiveValue_) {
        bestParameter_ = candidateParameter_;
        bestSoftConstraintsValue_ = candidateSoftConstraintsValue_;
        bestObjectiveValue_ = candidateObjectiveValue_;
      }
    } while(!isFinished() && !isTerminated());
  }

  void GridSearch::setSamplingFactors(
      const arma::Col<double>& samplingFactors) {
    if(samplingFactors.n_elem != optimisationProblem_->getNumberOfDimensions()) {
      throw std::logic_error("The number of dimensions of the sampling factors (" + std::to_string(samplingFactors.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(optimisationProblem_->getNumberOfDimensions()) + ").");
    } else if(arma::sum(samplingFactors) != 1.0) {
      throw std::logic_error("The sum of all sampling factors (" + std::to_string(arma::sum(samplingFactors)) + ") must be 1.");
    }

    samplingFactors_ = samplingFactors;
  }

  std::string GridSearch::to_string() const noexcept {
    return "GridSearch";
  }
}
