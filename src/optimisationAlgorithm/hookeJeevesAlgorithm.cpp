#include "mantella_bits/optimisationAlgorithm/hookeJeevesAlgorithm.hpp"

// C++ standard library
#include <algorithm>
#include <cmath>

// Mantella
#include "mantella_bits/assert.hpp"
#include "mantella_bits/optimisationProblem.hpp"
#include "mantella_bits/probability.hpp"

namespace mant {
  HookeJeevesAlgorithm::HookeJeevesAlgorithm()
      : OptimisationAlgorithm() {
    setNextParametersFunction([this] (
        const arma::Mat<double>& parameters,
        const arma::Col<double>& differences) {
      if (arma::any(differences < 0)) {
        previousBestParameters_.insert(bestParameter_);
      } else {
        stepSize_ *= stepSizeDecrease_;
      }

      arma::Mat<double> nextParameters;
      for (arma::uword n = 0; n < parameters.n_rows; ++n) {
        arma::Col<double> nextParameterCandidate = bestParameter_;
        
        nextParameterCandidate(n) += stepSize_;
        if (previousBestParameters_.find(nextParameterCandidate) == previousBestParameters_.cend()) {
          nextParameters.insert_cols(1, nextParameterCandidate);
        }
        
        nextParameterCandidate -= 2 * stepSize_;
        if (previousBestParameters_.find(nextParameterCandidate) == previousBestParameters_.cend()) {
          nextParameters.insert_cols(1, nextParameterCandidate);
        }
      }
      
      return nextParameters;
    });
  }
  
  void HookeJeevesAlgorithm::optimise(
      const std::shared_ptr<OptimisationProblem> optimisationProblem,
      const arma::Mat<double>& initialParameters) {
    verify(initialParameters.n_cols == 1, "optimise: The Hooke-Jeeves algorithm accepts only a single initial parameter.");
    
    if (!std::isfinite(initialStepSize_)) {
      setInitialStepSize(1);
    } else {
      // verify
    }
    
    if (!std::isfinite(stepSizeDecrease_)) {
      setStepSizeDecrease(0.5);
    } else {
      // verify
    }
    
    stepSize_ = initialStepSize_;
    OptimisationAlgorithm::optimise(optimisationProblem, initialParameters);
  }
  
  void HookeJeevesAlgorithm::optimise(
      const std::shared_ptr<OptimisationProblem> optimisationProblem) {
    optimise(optimisationProblem, arma::randu<arma::Col<double>>(optimisationProblem->numberOfDimensions_));
  }

  void HookeJeevesAlgorithm::setInitialStepSize(
      const double initialStepSize) {
    verify(initialStepSize > 0, "The initial step size must be strict greater than 0.");
      
    initialStepSize_ = initialStepSize;
  }

  void HookeJeevesAlgorithm::setStepSizeDecrease(
      const double stepSizeDecrease) {
    verify(stepSizeDecrease > 0 && stepSizeDecrease < 1, "The step size decrease must be within (0, 1)");
      
    stepSizeDecrease_ = stepSizeDecrease;
  }
}
