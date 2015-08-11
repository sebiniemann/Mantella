#include <mantella_bits/optimisationAlgorithm/trajectoryBasedOptimisationAlgorithm/simulatedAnnealing.hpp>

// C++ standard library
#include <random>

// Mantella
#include <mantella_bits/helper/assert.hpp>
#include <mantella_bits/helper/rng.hpp>

// TODO Add restarting
namespace mant {
  SimulatedAnnealing::SimulatedAnnealing(
      const std::shared_ptr<OptimisationProblem> optimisationProblem)
    : TrajectoryBasedOptimisationAlgorithm(optimisationProblem) {
    setMaximalStepSize(arma::norm(getLowerBounds() - getUpperBounds()) / 10.0);
  }

  void SimulatedAnnealing::optimiseImplementation() {
    ++numberOfIterations_;

    bestParameter_ = initialParameter_;
    bestSoftConstraintsValue_ = getSoftConstraintsValue(initialParameter_);
    bestObjectiveValue_ = getObjectiveValue(initialParameter_);

    arma::Col<double> state = bestParameter_;
    while(!isFinished() && !isTerminated()) {
      ++numberOfIterations_;

      const arma::Col<double>& candidateParameter = getRandomNeighbour(state, 0.0, maximalStepSize_);

      const double& candidateSoftConstraintsValue = getSoftConstraintsValue(candidateParameter);
      const double& candidateObjectiveValue = getObjectiveValue(candidateParameter);

      if (updateBestParameter(candidateParameter, candidateSoftConstraintsValue, candidateObjectiveValue) || isAcceptableState(candidateSoftConstraintsValue + candidateObjectiveValue)) {
        state = candidateParameter;
      }
    }
  }

  bool SimulatedAnnealing::isAcceptableState(
      const double candidateObjectiveValue) {
    return std::exp((bestObjectiveValue_ - candidateObjectiveValue) / (static_cast<double>(numberOfIterations_) * static_cast<double>(maximalNumberOfIterations_))) < std::uniform_real_distribution<double>(0.0, 1.0)(Rng::getGenerator());
  }

  void SimulatedAnnealing::setMaximalStepSize(
      const double maximalStepSize) {
    verify(maximalStepSize > 0.0, "The maximal step size must be strict greater than 0.");

    maximalStepSize_ = maximalStepSize;
  }

  std::string SimulatedAnnealing::toString() const {
    return "simulated_annealing";
  }
}
