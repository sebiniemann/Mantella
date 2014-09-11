#include <hop_bits/optimisationAlgorithm.hpp>

#include <limits>

namespace hop {
  OptimisationAlgorithm::OptimisationAlgorithm(const std::shared_ptr<OptimisationProblem> optimisationProblem)
    : optimisationProblem_(optimisationProblem) {

  }

  bool OptimisationAlgorithm::optimise() {
    bestObjectiveValue_ = std::numeric_limits<double>::infinity();
    bestSolution_ = arma::Col<double>({});
    numberOfIterations_ = 0;
    optimisationProblem_->reset();

    return optimiseImplementation();
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
    return (optimisationProblem_->getNumberOfEvaluations() >= optimisationProblem_->getMaximalNumberOfEvaluations());
  }

  unsigned int OptimisationAlgorithm::getNumberOfIterations() const {
    return numberOfIterations_;
  }
}
