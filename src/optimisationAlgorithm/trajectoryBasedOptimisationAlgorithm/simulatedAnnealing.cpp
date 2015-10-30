#include <mantella_bits/optimisationAlgorithm/trajectoryBasedOptimisationAlgorithm/simulatedAnnealing.hpp>

// C++ standard library
#include <random>
#include <cmath>

// Mantella
#include <mantella_bits/helper/assert.hpp>
#include <mantella_bits/helper/rng.hpp>

// TODO Add restarting
namespace mant {
  SimulatedAnnealing::SimulatedAnnealing(
      const std::shared_ptr<OptimisationProblem> optimisationProblem)
      : TrajectoryBasedOptimisationAlgorithm(optimisationProblem) {
    setMaximalStepSize((getUpperBounds() - getLowerBounds()) * 0.1);
  }

  void SimulatedAnnealing::optimiseImplementation() {
    ++numberOfIterations_;

    bestParameter_ = initialParameter_;
    bestObjectiveValue_ = getObjectiveValue(initialParameter_);

    arma::Col<double> state = bestParameter_;
    while (!isFinished() && !isTerminated()) {
      ++numberOfIterations_;

      const arma::Col<double>& candidateParameter = getRandomNeighbour(bestParameter_, arma::zeros<arma::Col<double>>(numberOfDimensions_), maximalStepSize_);
      const double candidateObjectiveValue = getObjectiveValue(candidateParameter);

      if (updateBestParameter(candidateParameter, candidateObjectiveValue) || isAcceptableState(candidateObjectiveValue)) {
        state = candidateParameter;
      }
    }
  }

  bool SimulatedAnnealing::isAcceptableState(
      const double candidateObjectiveValue) {
    return std::exp((bestObjectiveValue_ - candidateObjectiveValue) / (static_cast<double>(numberOfIterations_) * static_cast<double>(maximalNumberOfIterations_))) < std::uniform_real_distribution<double>(0.0, 1.0)(Rng::getGenerator());
  }

  void SimulatedAnnealing::setMaximalStepSize(
      const arma::Col<double>& maximalStepSize) {
    verify(maximalStepSize.n_elem == numberOfDimensions_,
        "The number of dimensions of the maximal step size must match the number of dimensions of the optimisation problem.");
    verify(arma::all(maximalStepSize > 0),
        "The maximal step size must be strict greater than 0 for each dimension.");

    maximalStepSize_ = maximalStepSize;
  }

  std::string SimulatedAnnealing::toString() const {
    return "simulated_annealing";
  }
}
