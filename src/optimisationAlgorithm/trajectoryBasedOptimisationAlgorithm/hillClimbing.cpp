#include <mantella_bits/optimisationAlgorithm/trajectoryBasedOptimisationAlgorithm/hillClimbing.hpp>

// Mantella
#include <mantella_bits/helper/assert.hpp>

// TODO Add restarting
namespace mant {
  HillClimbing::HillClimbing(
      const std::shared_ptr<OptimisationProblem> optimisationProblem)
      : TrajectoryBasedOptimisationAlgorithm(optimisationProblem) {
    setMinimalStepSize(arma::zeros<arma::Col<double>>(numberOfDimensions_));
    setMaximalStepSize((getUpperBounds() - getLowerBounds()) * 0.1);
  }

  void HillClimbing::optimiseImplementation() {
    ++numberOfIterations_;

    bestParameter_ = initialParameter_;
    bestObjectiveValue_ = getObjectiveValue(initialParameter_);

    while (!isFinished() && !isTerminated()) {
      ++numberOfIterations_;

      const arma::Col<double>& candidateParameter = getRandomNeighbour(bestParameter_, minimalStepSize_, maximalStepSize_);
      updateBestParameter(candidateParameter, getObjectiveValue(candidateParameter));
    }
  }

  void HillClimbing::setMinimalStepSize(
      const arma::Col<double>& minimalStepSize) {
    verify(minimalStepSize.n_elem == numberOfDimensions_,
        "The number of dimensions of the minimal step size must match the number of dimensions of the optimisation problem.");
    verify(arma::all(minimalStepSize >= 0),
        "The minimal step size must be at least 0 for each dimension.");

    minimalStepSize_ = minimalStepSize;
  }

  void HillClimbing::setMaximalStepSize(
      const arma::Col<double>& maximalStepSize) {
    verify(maximalStepSize.n_elem == numberOfDimensions_,
        "The number of dimensions of the maximal step size must match the number of dimensions of the optimisation problem.");
    verify(arma::all(maximalStepSize > 0),
        "The maximal step size must be strict greater than 0 for each dimension.");

    maximalStepSize_ = maximalStepSize;
  }

  std::string HillClimbing::toString() const {
    return "hill_climbing";
  }
}
