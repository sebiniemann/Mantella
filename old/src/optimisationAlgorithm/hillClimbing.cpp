#include "mantella_bits/optimisationAlgorithm/hillClimbing.hpp"

// C++ standard library
#include <cassert>
#include <functional>
#include <string>
#include <utility>

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationProblem.hpp"
#include "mantella_bits/probability.hpp"

namespace mant {
  HillClimbing::HillClimbing()
      : OptimisationAlgorithm() {
    bestFoundParameter_ = 2.0 * arma::ones<arma::vec>(3);

    setInitialisingFunctions(
        {{[this](
              const arma::uword numberOfDimensions_,
              const arma::mat& initialParameters_) {
            assert(getMinimalStepSize() <= getMaximalStepSize() && "HillClimbing.initialisingFunctions: The maximal step size must be must be greater than or equal to the minimal one.");

            return initialParameters_;
          },
          "Step size validation"}});

    setNextParametersFunctions(
        {{[this](
              const arma::uword numberOfDimensions_,
              const arma::mat& parameters_,
              const arma::rowvec& objectiveValues_,
              const arma::rowvec& differences_) {
            return randomNeighbour(getBestFoundParameter(), getMinimalStepSize(), getMaximalStepSize());
          },
          "Return a random neighbour of the best parameter within the step sizes"}});

    setMinimalStepSize(0.0);
    setMaximalStepSize(0.1);
  }

  void HillClimbing::optimise(
      OptimisationProblem& optimisationProblem) {
    OptimisationAlgorithm::optimise(optimisationProblem, uniformRandomNumbers(optimisationProblem.numberOfDimensions_));
  }

  void HillClimbing::setMinimalStepSize(
      const double minimalStepSize) {
    assert(minimalStepSize >= 0 && "HillClimbing.setMinimalStepSize: The minimal step size must be positive (including 0).");

    minimalStepSize_ = minimalStepSize;
  }

  double HillClimbing::getMinimalStepSize() const {
    return minimalStepSize_;
  }

  void HillClimbing::setMaximalStepSize(
      const double maximalStepSize) {
    assert(maximalStepSize >= 0 && "HillClimbing.setMaximalStepSize: The maximal step size must be positive (including 0).");

    maximalStepSize_ = maximalStepSize;
  }

  double HillClimbing::getMaximalStepSize() const {
    return maximalStepSize_;
  }
}
