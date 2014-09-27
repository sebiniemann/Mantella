#include <hop_bits/optimisationAlgorithm/hookeJeevesAlgorithm.hpp>

#include <random>

#include <hop_bits/helper/random.hpp>

namespace hop {
  HookeJeevesAlgorithm::HookeJeevesAlgorithm(const std::shared_ptr<OptimisationProblem> optimisationProblem)
    : OptimisationAlgorithm(optimisationProblem) {

  }

  void HookeJeevesAlgorithm::optimiseImplementation() {
    stepSize_ = optimisationProblem_->getUpperBounds() - optimisationProblem_->getLowerBounds();
    reduceStepSize_ = false;

    arma::Col<double> candidateSolution;
    do {
      candidateSolution = arma::randu<arma::Col<double>>(optimisationProblem_->getNumberOfDimensions()) % stepSize_ + optimisationProblem_->getLowerBounds();
    } while(!optimisationProblem_->isSatisfyingConstraints(candidateSolution));

    ++numberOfIterations_;
    bestObjectiveValue_ = optimisationProblem_->getObjectiveValue(candidateSolution) + optimisationProblem_->getSoftConstraintsValue(candidateSolution);
    bestSolution_ = candidateSolution;

    while(true) {
      if(reduceStepSize_) {
        stepSize_ /= 2;
      }

      reduceStepSize_ = true;
      arma::Col<double> candidateSolution = bestSolution_;
      for (std::size_t n = 0; n < optimisationProblem_->getNumberOfDimensions(); ++n) {

        candidateSolution.at(n) += stepSize_.at(n);
        if(optimisationProblem_->isSatisfyingConstraints(candidateSolution)) {
          ++numberOfIterations_;
          double objectiveValue = optimisationProblem_->getObjectiveValue(candidateSolution) + optimisationProblem_->getSoftConstraintsValue(candidateSolution);

          if (objectiveValue < bestObjectiveValue_) {
            bestObjectiveValue_ = objectiveValue;
            bestSolution_ = candidateSolution;

            reduceStepSize_ = false;
          }
        }

        if(isFinished() || isTerminated()) {
          return;
        }

        candidateSolution.at(n) -= 2 * stepSize_.at(n);
        if(optimisationProblem_->isSatisfyingConstraints(candidateSolution)) {
          ++numberOfIterations_;
          double objectiveValue = optimisationProblem_->getObjectiveValue(candidateSolution) + optimisationProblem_->getSoftConstraintsValue(candidateSolution);

          if (objectiveValue < bestObjectiveValue_) {
            bestObjectiveValue_ = objectiveValue;
            bestSolution_ = candidateSolution;

            reduceStepSize_ = false;
          }
        }

        if(isFinished() || isTerminated()) {
          return;
        }

        candidateSolution.at(n) += stepSize_.at(n);
      }
    }
  }
}
