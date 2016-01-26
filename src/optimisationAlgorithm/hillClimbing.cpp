#include "mantella_bits/optimisationAlgorithm/hillClimbing.hpp"

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
            const arma::uword numberOfDimensions_,
            const arma::Mat<double>& parameters_,
            const arma::Row<double>& objectiveValues_,
            const arma::Row<double>& differences_) {
          return randomNeighbour(bestParameter_, minimalStepSize_, maximalStepSize_);
        },
        "Hill climbing");

    setMinimalStepSize(0);
    setMaximalStepSize(0.1);
  }

  void HillClimbing::initialise(
      const arma::uword numberOfDimensions,
      const arma::Mat<double>& initialParameters) {
    verify(minimalStepSize_ <= maximalStepSize_, ""); // TODO
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

  double HillClimbing::getMinimalStepSize() const {
    return minimalStepSize_;
  }

  void HillClimbing::setMaximalStepSize(
      const double maximalStepSize) {
    verify(maximalStepSize > 0, "The maximal step size must be strict greater than 0 for each dimension.");

    maximalStepSize_ = maximalStepSize;
  }

  double HillClimbing::getMaximalStepSize() const {
    return maximalStepSize_;
  }
}
