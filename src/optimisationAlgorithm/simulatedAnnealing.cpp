#include "mantella_bits/optimisationAlgorithm/simulatedAnnealing.hpp"

// C++ standard library
#include <chrono>
#include <cmath>
#include <limits>
#include <random>

// Mantella
#include "mantella_bits/assert.hpp"
#include "mantella_bits/optimisationProblem.hpp"
#include "mantella_bits/probability.hpp"
#include "mantella_bits/randomNumberGenerator.hpp"

namespace mant {
  SimulatedAnnealing::SimulatedAnnealing()
      : OptimisationAlgorithm() {
    setNextParametersFunction(
        [this](
            const arma::uword numberOfDimensions_,
            const arma::Mat<double>& parameters_,
            const arma::Row<double>& objectiveValues_,
            const arma::Row<double>& differences_) {
          if (differences_(0) < 0) {
            state_ = bestParameter_;
          } else if (isAcceptableStateFunction_(differences_(0))) {
            state_ = parameters_.col(0);
          }

          return randomNeighbour(state_, minimalStepSize_, maximalStepSize_);
        },
        "Simulated annealing");

    setIsAcceptableStateFunction(
        [this](
            const double objectiveValue) {
          double progress;
          if (maximalNumberOfIterations_ < std::numeric_limits<arma::uword>::max()) {
            progress = static_cast<double>(numberOfIterations_ / maximalNumberOfIterations_);
          } else {
            progress = static_cast<double>(duration_.count() / maximalDuration_.count());
          }

          return std::exp((bestObjectiveValue_ - objectiveValue) / std::exp(progress)) < std::uniform_real_distribution<double>(0.0, 1.0)(Rng::getGenerator());
        },
        "Exponential");

    setMinimalStepSize(0);
    setMaximalStepSize(0.1);
  }

  void SimulatedAnnealing::initialise(
      const arma::uword numberOfDimensions,
      const arma::Mat<double>& initialParameters) {
    verify(minimalStepSize_ <= maximalStepSize_, "SimulatedAnnealing.initialise: The minimal step size must be less than or equal to the maximal one.");
  }

  void SimulatedAnnealing::optimise(
      OptimisationProblem& optimisationProblem) {
    optimise(optimisationProblem, arma::randu<arma::Col<double>>(optimisationProblem.numberOfDimensions_));
  }

  void SimulatedAnnealing::setIsAcceptableStateFunction(
      std::function<bool(const double objectiveValue)> isAcceptableStateFunction,
      const std::string& isAcceptableStateFunctionName) {
    verify(static_cast<bool>(isAcceptableStateFunction), "SimulatedAnnealing.setIsAcceptableStateFunction: The is-acceptable-state function must be callable.");

    isAcceptableStateFunction_ = isAcceptableStateFunction;
    isAcceptableStateFunctionName_ = isAcceptableStateFunctionName;
  }

  void SimulatedAnnealing::setIsAcceptableStateFunction(
      std::function<bool(const double objectiveValue)> isAcceptableStateFunction) {
    setIsAcceptableStateFunction(isAcceptableStateFunction, "Unnamed, custom is-acceptable-state function");
  }

  std::string SimulatedAnnealing::getIsAcceptableStateFunctionName() const {
    return isAcceptableStateFunctionName_;
  }

  void SimulatedAnnealing::setMinimalStepSize(
      const double minimalStepSize) {
    verify(minimalStepSize >= 0, "SimulatedAnnealing.setMinimalStepSize: The minimal step size must be positive (including 0).");

    minimalStepSize_ = minimalStepSize;
  }

  double SimulatedAnnealing::getMinimalStepSize() const {
    return minimalStepSize_;
  }

  void SimulatedAnnealing::setMaximalStepSize(
      const double maximalStepSize) {
    verify(maximalStepSize > 0, "SimulatedAnnealing.setMaximalStepSize: The maximal step size must be strict greater than 0.");

    maximalStepSize_ = maximalStepSize;
  }

  double SimulatedAnnealing::getMaximalStepSize() const {
    return maximalStepSize_;
  }
}
