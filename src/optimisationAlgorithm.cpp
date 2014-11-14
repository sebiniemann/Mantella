#include <hop_bits/optimisationAlgorithm.hpp>

// C++ Standard Library
#include <limits>

namespace hop {
  OptimisationAlgorithm::OptimisationAlgorithm(
      const std::shared_ptr<OptimisationProblem> optimisationProblem)
    : optimisationProblem_(optimisationProblem),
      bestObjectiveValue_(std::numeric_limits<double>::infinity()),
      numberOfIterations_(0) {
    setMaximalNumberOfIterations(1000);
  }

  void OptimisationAlgorithm::optimise() {
    // Reset results, counters and caches
    bestObjectiveValue_ = std::numeric_limits<double>::infinity();
    bestParameter_.reset();
    numberOfIterations_ = 0;
    optimisationProblem_->reset();

    return optimiseImplementation();
  }

  unsigned int OptimisationAlgorithm::getNumberOfIterations() const noexcept {
    return numberOfIterations_;
  }

  void OptimisationAlgorithm::setMaximalNumberOfIterations(
      const unsigned int& maximalNumberOfIterations) noexcept {
    maximalNumberOfIterations_ = maximalNumberOfIterations;
  }

  arma::Col<double> OptimisationAlgorithm::getBestParameter() const noexcept {
    return bestParameter_;
  }

  double OptimisationAlgorithm::getBestObjectiveValue() const noexcept {
    return bestObjectiveValue_;
  }

  double OptimisationAlgorithm::getBestSoftConstraintValue() const noexcept {
    return bestSoftConstraintValue_;
  }

  bool OptimisationAlgorithm::isFinished() const noexcept {
    return (bestObjectiveValue_ <= optimisationProblem_->getAcceptableObjectiveValue());
  }

  bool OptimisationAlgorithm::isTerminated() const noexcept {
    return (numberOfIterations_ >= maximalNumberOfIterations_);
  }
}
