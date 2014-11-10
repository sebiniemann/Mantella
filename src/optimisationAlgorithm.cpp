#include <hop_bits/optimisationAlgorithm.hpp>

// C++ Standard Library
#include <limits>

namespace hop {
  OptimisationAlgorithm::OptimisationAlgorithm(
      const std::shared_ptr<OptimisationProblem> optimisationProblem)
    : optimisationProblem_(optimisationProblem) {
    setMaximalNumberOfIterations(1000);
    reset();
  }

  void OptimisationAlgorithm::optimise() {
    reset();
    optimiseImplementation();
  }

  double OptimisationAlgorithm::getBestObjectiveValue() const {
    return bestObjectiveValue_;
  }

  arma::Col<double> OptimisationAlgorithm::getBestSolution() const {
    return bestSolution_;
  }

  bool OptimisationAlgorithm::isFinished() const {
    return (bestObjectiveValue_ <= optimisationProblem_->getAcceptableObjectiveValue());
  }

  bool OptimisationAlgorithm::isTerminated() const {
    return (numberOfIterations_ >= maximalNumberOfIterations_);
  }

  unsigned int OptimisationAlgorithm::getNumberOfIterations() const {
    return numberOfIterations_;
  }

  void OptimisationAlgorithm::setMaximalNumberOfIterations(
      const unsigned int& maximalNumberOfIterations) {
    maximalNumberOfIterations_ = maximalNumberOfIterations;
  }

  void OptimisationAlgorithm::reset() {
    bestObjectiveValue_ = std::numeric_limits<double>::infinity();
    bestSolution_.fill(arma::datum::nan);
    numberOfIterations_ = 0;
    optimisationProblem_->reset();
  }
}
