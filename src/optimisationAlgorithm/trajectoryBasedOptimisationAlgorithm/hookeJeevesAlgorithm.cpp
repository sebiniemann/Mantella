#include <mantella_bits/optimisationAlgorithm/trajectoryBasedOptimisationAlgorithm/hookeJeevesAlgorithm.hpp>

// Mantella
#include <mantella_bits/helper/assert.hpp>

// TODO Add restarting
namespace mant {
  HookeJeevesAlgorithm::HookeJeevesAlgorithm(
      const std::shared_ptr<OptimisationProblem> optimisationProblem)
      : TrajectoryBasedOptimisationAlgorithm(optimisationProblem) {
    setInitialStepSize(getUpperBounds() - getLowerBounds());
    setStepSizeDecrease(arma::ones<arma::Col<double>>(optimisationProblem->numberOfDimensions_) * 0.5);
  }

  void HookeJeevesAlgorithm::optimiseImplementation() {
    ++numberOfIterations_;

    bestParameter_ = initialParameter_;
    bestObjectiveValue_ = getObjectiveValue(initialParameter_);

    bool reduceStepSize = false;
    arma::Col<double> stepSize = initialStepSize_;

    while (!isFinished() && !isTerminated()) {
      if (reduceStepSize) {
        stepSize %= stepSizeDecrease_;
      }

      reduceStepSize = true;

      arma::Col<double> candidateParameter = bestParameter_;
      for (arma::uword n = 0; n < numberOfDimensions_; ++n) {
        candidateParameter(n) += stepSize(n);

        if (getUpperBounds()(n) < candidateParameter(n)) {
          candidateParameter(n) = getUpperBounds()(n);
        }

        ++numberOfIterations_;
        double candidateObjectiveValue = getObjectiveValue(candidateParameter);

        if (updateBestParameter(candidateParameter, candidateObjectiveValue)) {
          reduceStepSize = false;
        }

        if (isFinished() || isTerminated()) {
          break;
        }

        candidateParameter(n) = bestParameter_(n) - stepSize(n);

        if (getLowerBounds()(n) > candidateParameter(n)) {
          candidateParameter(n) = getLowerBounds()(n);
        }

        ++numberOfIterations_;
        candidateObjectiveValue = getObjectiveValue(candidateParameter);

        if (updateBestParameter(candidateParameter, candidateObjectiveValue)) {
          reduceStepSize = false;
        }

        if (isFinished() || isTerminated()) {
          break;
        }

        candidateParameter(n) = bestParameter_(n);
      }
    }
  }

  void HookeJeevesAlgorithm::setInitialStepSize(
      const arma::Col<double>& initialStepSize) {
    verify(initialStepSize.n_elem == numberOfDimensions_,
        "The number of dimensions of the initial step size must match the number of dimensions of the optimisation problem.");
    verify(arma::all(initialStepSize > 0),
        "The initial step size must be strict greater than 0.");

    initialStepSize_ = initialStepSize;
  }

  void HookeJeevesAlgorithm::setStepSizeDecrease(
      const arma::Col<double>& stepSizeDecrease) {
    verify(stepSizeDecrease.n_elem == numberOfDimensions_,
        "The number of dimensions of the step size decrease must match the number of dimensions of the optimisation problem.");
    verify(arma::all(stepSizeDecrease > 0) && arma::all(stepSizeDecrease < 1),
        "The step size decrease must be within (0, 1)");

    stepSizeDecrease_ = stepSizeDecrease;
  }

  std::string HookeJeevesAlgorithm::toString() const {
    return "hooke_jeeves_algorithm";
  }
}
