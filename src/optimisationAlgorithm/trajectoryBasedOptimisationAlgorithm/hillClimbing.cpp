#include <mantella_bits/optimisationAlgorithm/trajectoryBasedOptimisationAlgorithm/hillClimbing.hpp>

// Mantella
#include <mantella_bits/helper/assert.hpp>

// TODO Add restarting
namespace mant {
  HillClimbing::HillClimbing(
      const std::shared_ptr<OptimisationProblem> optimisationProblem)
    : TrajectoryBasedOptimisationAlgorithm(optimisationProblem) {
    setMaximalStepSize(arma::norm(getLowerBounds() - getUpperBounds()) / 10.0);
  }

  void HillClimbing::optimiseImplementation() {
    ++numberOfIterations_;

    bestParameter_ = initialParameter_;
    bestObjectiveValue_ = getObjectiveValue(initialParameter_);

    while(!isFinished() && !isTerminated()) {
      ++numberOfIterations_;

      const arma::Col<double>& candidateParameter = getRandomNeighbour(bestParameter_, 0.0, maximalStepSize_);

      updateBestParameter(candidateParameter, getObjectiveValue(candidateParameter));
    }
  }

  void HillClimbing::setMaximalStepSize(
      const double maximalStepSize) {
    if (maximalStepSize <= 0) {
      throw std::logic_error("The maximal step size must be strict greater than 0.");
    }

    maximalStepSize_ = maximalStepSize;
  }

  std::string HillClimbing::toString() const {
    return "hill_climbing";
  }
}
