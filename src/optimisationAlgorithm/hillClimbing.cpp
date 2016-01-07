#include "mantella_bits/optimisationAlgorithm/hillClimbing.hpp"

// C++ standard library
#include <cmath>

// Mantella
#include "mantella_bits/assert.hpp"
#include "mantella_bits/optimisationProblem.hpp"
#include "mantella_bits/probability.hpp"

namespace mant {
  HillClimbing::HillClimbing()
      : OptimisationAlgorithm(),
        minimalStepSize_(arma::datum::nan),
        maximalStepSize_(arma::datum::nan) {
    setNextParametersFunction(
        [this](
            const arma::uword numberOfDimensions,
            const arma::Mat<double>& parameters,
            const arma::Col<double>& objectiveValues,
            const arma::Col<double>& differences) {
          return randomNeighbour(bestParameter_, minimalStepSize_, maximalStepSize_);
        },
        "Hill climbing");
  }

  void HillClimbing::initialise(
      const arma::uword numberOfDimensions,
      const arma::Mat<double>& initialParameters) {
    if (!std::isfinite(minimalStepSize_)) {
      setMinimalStepSize(0);
    } else {
      // verify
    }

    if (!std::isfinite(maximalStepSize_)) {
      setMaximalStepSize(0.1);
    } else {
      // verify
    }

    // verify minimalStepSize_ <= maximalStepSize_
  }

  void HillClimbing::optimise(
      OptimisationProblem& optimisationProblem) {
    OptimisationAlgorithm::optimise(optimisationProblem, arma::randu<arma::Col<double>>(optimisationProblem.numberOfDimensions_));
  }

  void HillClimbing::setMinimalStepSize(
      const double minimalStepSize) {
    verify(minimalStepSize >= 0, "The minimal step size must be at least 0 for each dimension.");

    minimalStepSize_ = minimalStepSize;
  }

  void HillClimbing::setMaximalStepSize(
      const double maximalStepSize) {
    verify(maximalStepSize > 0, "The maximal step size must be strict greater than 0 for each dimension.");

    maximalStepSize_ = maximalStepSize;
  }
}
