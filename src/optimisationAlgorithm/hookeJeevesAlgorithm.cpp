#include "mantella_bits/optimisationAlgorithm/hookeJeevesAlgorithm.hpp"

// Mantella
#include "mantella_bits/assert.hpp"
#include "mantella_bits/optimisationProblem.hpp"

namespace mant {
  HookeJeevesAlgorithm::HookeJeevesAlgorithm()
      : OptimisationAlgorithm(),
        stepSize_(0) {
    setNextParametersFunction(
        [this](
            const arma::uword numberOfDimensions_,
            const arma::Mat<double>& parameters_,
            const arma::Row<double>& objectiveValues_,
            const arma::Row<double>& differences_) {
          if (arma::all(differences_ >= 0)) {
            stepSize_ /= stepSizeDecrease_;
          }

          arma::Mat<double> nextParameters(numberOfDimensions_, 2 * numberOfDimensions_);
          for (arma::uword n = 0; n < numberOfDimensions_; ++n) {
            arma::Col<double> nextParameterCandidate = bestParameter_;
            
            nextParameterCandidate(n) += stepSize_;
            nextParameters.col(2 * n) = nextParameterCandidate;
            
            nextParameterCandidate(n) -= 2 * stepSize_;
            nextParameters.col(2 * n + 1) = nextParameterCandidate;
          }
          
          return nextParameters;
        },
        "Hooke-Jeeves algorithm");

    setInitialStepSize(1.0);
    setStepSizeDecrease(0.5);
  }

  void HookeJeevesAlgorithm::initialise(
      const arma::uword numberOfDimensions,
      const arma::Mat<double>& initialParameters) {
    stepSize_ = initialStepSize_;
  }

  void HookeJeevesAlgorithm::optimise(
      OptimisationProblem& optimisationProblem) {
    optimise(optimisationProblem, arma::randu<arma::Col<double>>(optimisationProblem.numberOfDimensions_));
  }

  void HookeJeevesAlgorithm::setInitialStepSize(
      const double initialStepSize) {
    verify(initialStepSize > 0, "HookeJeevesAlgorithm.setInitialStepSize: The initial step size must be strict greater than 0.");

    initialStepSize_ = initialStepSize;
  }

  double HookeJeevesAlgorithm::getInitialStepSize() const {
    return initialStepSize_;
  }

  void HookeJeevesAlgorithm::setStepSizeDecrease(
      const double stepSizeDecrease) {
    verify(stepSizeDecrease > 0 && stepSizeDecrease < 1, "HookeJeevesAlgorithm.setStepSizeDecrease: The step size decrease must be within the interval (0, 1).");

    stepSizeDecrease_ = stepSizeDecrease;
  }

  double HookeJeevesAlgorithm::getStepSizeDecrease() const {
    return stepSizeDecrease_;
  }
}
