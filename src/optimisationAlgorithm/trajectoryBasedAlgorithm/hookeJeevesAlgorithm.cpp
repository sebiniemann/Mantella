#include <hop_bits/optimisationAlgorithm/trajectoryBasedAlgorithm/hookeJeevesAlgorithm.hpp>

// HOP
#include <hop_bits/helper/random.hpp>

namespace hop {
  HookeJeevesAlgorithm::HookeJeevesAlgorithm(
      const std::shared_ptr<OptimisationProblem> optimisationProblem) noexcept
    : TrajectoryBasedAlgorithm(optimisationProblem),
      reduceStepSize_(false),
      candidateObjectiveValue_(std::numeric_limits<double>::infinity()),
      candidateSoftConstraintValue_(std::numeric_limits<double>::infinity()) {
    setInitialStepSize(optimisationProblem_->getUpperBounds() - optimisationProblem_->getLowerBounds());
  }

  void HookeJeevesAlgorithm::optimiseImplementation() noexcept {
    ++numberOfIterations_;

    bestParameter_ = initialParameter_;
    bestSoftConstraintValue_ = optimisationProblem_->getSoftConstraintsValue(initialParameter_);
    bestObjectiveValue_ = optimisationProblem_->getObjectiveValue(initialParameter_);

    reduceStepSize_ = false;
    stepSize_ = initialStepSize_;
                ;
    while(!isFinished() && !isTerminated()) {
      if (reduceStepSize_) {
        stepSize_ = getReducedStepSize();
      }

      reduceStepSize_ = true;

      candidateParameter_ = bestParameter_;
      for (std::size_t n = 0; n < optimisationProblem_->getNumberOfDimensions(); ++n) {
        candidateParameter_.at(n) += stepSize_.at(n);

        // TODO Add proper out of bound handling
        if(optimisationProblem_->getLowerBounds().at(n) <= candidateParameter_.at(n) && candidateParameter_.at(n) <= optimisationProblem_->getUpperBounds().at(n)) {
          ++numberOfIterations_;
          candidateSoftConstraintValue_ = optimisationProblem_->getSoftConstraintsValue(candidateParameter_);
          candidateObjectiveValue_ = optimisationProblem_->getObjectiveValue(candidateParameter_);

          if(candidateSoftConstraintValue_ < bestSoftConstraintValue_ || candidateSoftConstraintValue_ == bestSoftConstraintValue_ && candidateObjectiveValue_ < bestObjectiveValue_) {
            reduceStepSize_ = false;

            bestParameter_ = candidateParameter_;
            bestSoftConstraintValue_ = candidateSoftConstraintValue_;
            bestObjectiveValue_ = candidateObjectiveValue_;
          }

          if (isFinished() || isTerminated()) {
            break;
          }
        }

        candidateParameter_.at(n) -= 2.0 * stepSize_.at(n);

        if(optimisationProblem_->getLowerBounds().at(n) <= candidateParameter_.at(n) && candidateParameter_.at(n) <= optimisationProblem_->getUpperBounds().at(n)) {
          ++numberOfIterations_;
          candidateSoftConstraintValue_ = optimisationProblem_->getSoftConstraintsValue(candidateParameter_);
          candidateObjectiveValue_ = optimisationProblem_->getObjectiveValue(candidateParameter_);

          if(candidateSoftConstraintValue_ < bestSoftConstraintValue_ || candidateSoftConstraintValue_ == bestSoftConstraintValue_ && candidateObjectiveValue_ < bestObjectiveValue_) {
            reduceStepSize_ = false;

            bestParameter_ = candidateParameter_;
            bestSoftConstraintValue_ = candidateSoftConstraintValue_;
            bestObjectiveValue_ = candidateObjectiveValue_;
          }

          if (isFinished() || isTerminated()) {
            break;
          }
        }

        candidateParameter_.at(n) += stepSize_.at(n);
      }
    }
  }

  arma::Col<double> HookeJeevesAlgorithm::getReducedStepSize() noexcept {
    return stepSize_ / 2;
  }

  void HookeJeevesAlgorithm::setInitialStepSize(
      const arma::Col<double>& initialStepSize) {
    if(initialStepSize.n_rows != optimisationProblem_->getNumberOfDimensions()) {
      throw std::logic_error("The number of dimensions of the initial step size (" + std::to_string(initialStepSize.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(optimisationProblem_->getNumberOfDimensions()) + ").");
    }

    initialStepSize_ = initialStepSize;
  }

  std::string HookeJeevesAlgorithm::to_string() const noexcept {
    return "HookeJeevesAlgorithm";
  }
}
