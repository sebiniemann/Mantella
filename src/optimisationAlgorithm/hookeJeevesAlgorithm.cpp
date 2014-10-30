#include <hop_bits/optimisationAlgorithm/hookeJeevesAlgorithm.hpp>

// HOP
#include <hop_bits/helper/random.hpp>

namespace hop {
  HookeJeevesAlgorithm::HookeJeevesAlgorithm(const std::shared_ptr<OptimisationProblem> optimisationProblem)
    : OptimisationAlgorithm(optimisationProblem) {
    setInitialStepSize(optimisationProblem_->getUpperBounds() - optimisationProblem_->getLowerBounds());
  }

  void HookeJeevesAlgorithm::optimiseImplementation() {
    bool reduceStepSize = false;

    arma::Col<double> candidateSolution;
    do {
      candidateSolution = arma::randu<arma::Col<double>>(optimisationProblem_->getNumberOfDimensions()) % stepSize_ + optimisationProblem_->getLowerBounds();
    } while(!optimisationProblem_->isSatisfyingConstraints(candidateSolution));

    ++numberOfIterations_;
    bestSolution_ = candidateSolution;
    bestObjectiveValue_ = optimisationProblem_->getObjectiveValue(candidateSolution) + optimisationProblem_->getSoftConstraintsValue(candidateSolution);

    while(!isFinished() && !isTerminated()) {
      if(reduceStepSize) {
        stepSize_ /= 2;
      }

      reduceStepSize = true;
      arma::Col<double> candidateSolution = bestSolution_;
      for (std::size_t n = 0; n < optimisationProblem_->getNumberOfDimensions(); ++n) {

        candidateSolution.at(n) += stepSize_.at(n);
        if(optimisationProblem_->isSatisfyingConstraints(candidateSolution)) {
          ++numberOfIterations_;
          double objectiveValue = optimisationProblem_->getObjectiveValue(candidateSolution) + optimisationProblem_->getSoftConstraintsValue(candidateSolution);

          if (objectiveValue < bestObjectiveValue_) {
            reduceStepSize = false;

            bestSolution_ = candidateSolution;
            bestObjectiveValue_ = objectiveValue;
          }
        }

        if(isFinished() || isTerminated()) {
          break;
        }

        candidateSolution.at(n) -= 2 * stepSize_.at(n);
        if(optimisationProblem_->isSatisfyingConstraints(candidateSolution)) {
          ++numberOfIterations_;
          double objectiveValue = optimisationProblem_->getObjectiveValue(candidateSolution) + optimisationProblem_->getSoftConstraintsValue(candidateSolution);

          if (objectiveValue < bestObjectiveValue_) {
            reduceStepSize = false;

            bestSolution_ = candidateSolution;
            bestObjectiveValue_ = objectiveValue;
          }
        }

        if(isFinished() || isTerminated()) {
          break;
        }

        candidateSolution.at(n) += stepSize_.at(n);
      }
    }
  }

  void HookeJeevesAlgorithm::setInitialStepSize(const arma::Col<double>& stepSize) {
    stepSize_ = stepSize;
  }

  std::string HookeJeevesAlgorithm::to_string() const {
    return "HookeJeevesAlgorithm";
  }
}
