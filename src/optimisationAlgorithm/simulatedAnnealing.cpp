#include "mantella_bits/optimisationAlgorithm/simulatedAnnealing.hpp"

// Mantella
#include "mantella_bits/assert.hpp"
#include "mantella_bits/probability.hpp"
#include "mantella_bits/randomNumberGenerator.hpp"

namespace mant {
  SimulatedAnnealing::SimulatedAnnealing()
      : OptimisationAlgorithm() {
    setNextParametersFunction([this] (
        const arma::Mat<double>& parameters,
        const arma::Col<double>& differences) {
      if(differences(0) < 0) {
        state_ = bestParameter_; 
      } else if (isAcceptableStateFunction_(differences(0))) {
        state_ = parameters.col(0);
      }
      
      return randomNeighbour(state_, minimalStepSize_, maximalStepSize_);
    });
  }
  
  void SimulatedAnnealing::optimise(
      OptimisationProblem& optimisationProblem,
      const arma::Mat<double>& initialParameters) {
    verify(initialParameters.n_cols == 1, "optimise: The simulated annealing algorithm accepts only a single initial parameter.");
      
    if (!static_cast<bool>(isAcceptableStateFunction_)) {
      setIsAcceptableStateFunction([this] (
          const double objectiveValue) {
        double progress;
        if (maximalNumberOfIterations_ < std::numeric_limits<arma::uword>::max()) {
          progress = static_cast<double>(numberOfIterations_ / maximalNumberOfIterations_);
        } else {
          progress = static_cast<double>(duration_.count() / maximalDuration_.count());
        }
        
        return std::exp((bestObjectiveValue_ - objectiveValue) / std::exp(progress)) < std::uniform_real_distribution<double>(0.0, 1.0)(Rng::getGenerator());
      });
    }
    
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
    
    OptimisationAlgorithm::optimise(optimisationProblem, initialParameters);
  }
  
  void SimulatedAnnealing::optimise(
      OptimisationProblem& optimisationProblem) {
    optimise(optimisationProblem, arma::randu<arma::Col<double>>(optimisationProblem.numberOfDimensions_));
  }

  void SimulatedAnnealing::setIsAcceptableStateFunction(
      std::function<bool(const double objectiveValue)> isAcceptableStateFunction) {
    verify(static_cast<bool>(isAcceptableStateFunction), "setIsAcceptableStateFunction: The function deciding whether or not an non-optimal state is acceptable must be callable.");
      
    isAcceptableStateFunction_ = isAcceptableStateFunction;
  }

  void SimulatedAnnealing::setMinimalStepSize(
      const double minimalStepSize) {
    verify(minimalStepSize >= 0, "The minimal step size must be at least 0 for each dimension.");
      
    minimalStepSize_ = minimalStepSize;
  }

  void SimulatedAnnealing::setMaximalStepSize(
      const double maximalStepSize) {
    verify(maximalStepSize > 0, "The maximal step size must be strict greater than 0 for each dimension.");
      
    maximalStepSize_ = maximalStepSize;
  }
}