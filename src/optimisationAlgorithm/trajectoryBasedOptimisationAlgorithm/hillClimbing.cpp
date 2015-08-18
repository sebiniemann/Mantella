#include <mantella_bits/optimisationAlgorithm/trajectoryBasedOptimisationAlgorithm/hillClimbing.hpp>

// Mantella
#include <mantella_bits/helper/assert.hpp>

// TODO Add restarting
namespace mant {
  HillClimbing::HillClimbing(
      const std::shared_ptr<OptimisationProblem> optimisationProblem)
    : TrajectoryBasedOptimisationAlgorithm(optimisationProblem) {
    setMaximalStepSize((getLowerBounds() - getUpperBounds()) * 0.1);
  }

  void HillClimbing::optimiseImplementation() {
    ++numberOfIterations_;

    bestParameter_ = initialParameter_;
    bestObjectiveValue_ = getObjectiveValue(initialParameter_);

    while(!isFinished() && !isTerminated()) {
      ++numberOfIterations_;

      const arma::Col<double>& candidateParameter = getRandomNeighbour(bestParameter_, arma::zeros<arma::Col<double>>(numberOfDimensions_), maximalStepSize_);
      updateBestParameter(candidateParameter, getObjectiveValue(candidateParameter));
    }
  }

  void HillClimbing::setMaximalStepSize(
      const arma::Col<double>& maximalStepSize) {
    verify(arma::all(maximalStepSize > 0), "The maximal step size must be strict greater than 0 for each dimension.");

    maximalStepSize_ = maximalStepSize;
  }

  std::string HillClimbing::toString() const {
    return "hill_climbing";
  }
}
