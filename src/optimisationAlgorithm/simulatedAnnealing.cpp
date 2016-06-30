#include "mantella_bits/optimisationAlgorithm/simulatedAnnealing.hpp"

// C++ standard library
#include <chrono>
#include <cmath>
#include <functional>
#include <random>
#include <ratio>
#include <stdexcept>
#include <string>
#include <utility>

// Mantella
#include "mantella_bits/optimisationProblem.hpp"
#include "mantella_bits/probability.hpp"
#include "mantella_bits/randomNumberGenerator.hpp"

namespace mant {
  SimulatedAnnealing::SimulatedAnnealing()
      : OptimisationAlgorithm() {
    setInitialisingFunctions({{[this](
                                   const arma::uword numberOfDimensions_,
                                   const arma::mat& initialParameters_) {
                                 if (minimalStepSize_ > maximalStepSize_) {
                                   throw std::logic_error("SimulatedAnnealing.initialisingFunctions: The maximal step size must be must be greater than or equal to the minimal one.");
                                 }

                                 return initialParameters_;
                               },
        "Step size validation"}});

    setNextParametersFunctions({{[this](
                                     const arma::uword numberOfDimensions_,
                                     const arma::mat& parameters_,
                                     const arma::rowvec& objectiveValues_,
                                     const arma::rowvec& differences_) {
                                   if (differences_(0) < 0) {
                                     state_ = getBestFoundParameter();
                                   } else {
                                     double durationEnergy = getUsedDuration() / getMaximalDuration();
                                     double numberOfIterationsEnergy = getUsedNumberOfIterations() / getMaximalNumberOfIterations();

                                     double energy;
                                     if (durationEnergy > numberOfIterationsEnergy) {
                                       energy = std::exp(-(objectiveValues_(0) - getBestFoundObjectiveValue()) / (1.0 - durationEnergy));
                                     } else {
                                       energy = std::exp(-(objectiveValues_(0) - getBestFoundObjectiveValue()) / (1.0 - numberOfIterationsEnergy));
                                     }

                                     if (std::bernoulli_distribution(energy)(Rng::generator_)) {
                                       state_ = parameters_.col(0);
                                     }
                                   }

                                   return randomNeighbour(state_, minimalStepSize_, maximalStepSize_);
                                 },
        "Simulated annealing"}});

    setMinimalStepSize(0);
    setMaximalStepSize(0.1);
  }

  void SimulatedAnnealing::optimise(
      OptimisationProblem& optimisationProblem) {
    // Objects like `optimisationProblem` perform all validations by themselves.
    optimise(optimisationProblem, arma::randu<arma::vec>(optimisationProblem.numberOfDimensions_));
  }

  void SimulatedAnnealing::setMinimalStepSize(
      const double minimalStepSize) {
    if (minimalStepSize < 0) {
      throw std::domain_error("SimulatedAnnealing.setMinimalStepSize: The minimal step size must be positive (including 0).");
    }

    minimalStepSize_ = minimalStepSize;
  }

  double SimulatedAnnealing::getMinimalStepSize() const {
    return minimalStepSize_;
  }

  void SimulatedAnnealing::setMaximalStepSize(
      const double maximalStepSize) {
    if (maximalStepSize <= 0) {
      throw std::domain_error("SimulatedAnnealing.setMaximalStepSize: The maximal step size must be positive (excluding 0).");
    }

    maximalStepSize_ = maximalStepSize;
  }

  double SimulatedAnnealing::getMaximalStepSize() const {
    return maximalStepSize_;
  }
}
