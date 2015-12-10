#include "mantella_bits/optimisationAlgorithm/hillClimbing.hpp"

// Mantella
#include "mantella_bits/assert.hpp"
#include "mantella_bits/probability.hpp"

namespace mant {
  HillClimbing::HillClimbing()
      : OptimisationAlgorithm(),
        minimalStepSize_(arma::datum::nan),
        maximalStepSize_(arma::datum::nan)  {
    setNextParametersFunction([this] (
        const arma::Mat<double>& parameters,
        const arma::Col<double>& differences) {
      return randomNeighbour(bestParameter_, minimalStepSize_, maximalStepSize_);
    });
  }
  
  void HillClimbing::optimise(
      OptimisationProblem& optimisationProblem,
      const arma::Mat<double>& initialParameters) {
    verify(initialParameters.n_cols == 1, "optimise: The hill climbing algorithm accepts only a single initial parameter.");
    
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
    
    OptimisationAlgorithm::optimise(optimisationProblem, initialParameters);
  }
  
  void HillClimbing::optimise(
      OptimisationProblem& optimisationProblem) {
    optimise(optimisationProblem, arma::randu<arma::Col<double>>(optimisationProblem.numberOfDimensions_));
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
