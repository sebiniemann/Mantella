#include "mantella_bits/optimisationAlgorithm/hookeJeevesAlgorithm.hpp"

// C++ standard library
#include <functional>
#include <stdexcept>
#include <string>
#include <utility>

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationProblem.hpp"

namespace mant {
  HookeJeevesAlgorithm::HookeJeevesAlgorithm()
      : OptimisationAlgorithm(),
        stepSize_(0) {
    setInitialisingFunctions({{[this](
                                   const arma::uword numberOfDimensions_,
                                   const arma::mat& initialParameters_) {
                                 stepSize_ = getInitialStepSize();

                                 return initialParameters_;
                               },
        "Reset step size to the initial one"}});

    setNextParametersFunctions({{[this](
                                     const arma::uword numberOfDimensions_,
                                     const arma::mat& parameters_,
                                     const arma::rowvec& objectiveValues_,
                                     const arma::rowvec& differences_) {
                                   if (arma::all(differences_ >= 0)) {
                                     stepSize_ *= getStepSizeDecrease();
                                   }

                                   return parameters_;
                                 },
                                    "Decrease the step size if the last objective values where worse than the best one"},
        {[this](
             const arma::uword numberOfDimensions_,
             const arma::mat& parameters_,
             const arma::rowvec& objectiveValues_,
             const arma::rowvec& differences_) {
                                  arma::mat nextParameters(numberOfDimensions_, 2 * numberOfDimensions_);
                                  for (arma::uword n = 0; n < numberOfDimensions_; ++n) {
                                    arma::vec nextParameterCandidate = getBestFoundParameter();

                                    nextParameterCandidate(n) += stepSize_;
                                    nextParameters.col(2 * n) = nextParameterCandidate;

                                    nextParameterCandidate(n) -= 2 * stepSize_;
                                    nextParameters.col(2 * n + 1) = nextParameterCandidate;
                                  }

                                  return nextParameters;
                                },
            "Hooke-Jeeves algorithm"}});

    setInitialStepSize(1.0);
    setStepSizeDecrease(0.5);
  }

  void HookeJeevesAlgorithm::optimise(
      OptimisationProblem& optimisationProblem) {
    optimise(optimisationProblem, arma::randu<arma::vec>(optimisationProblem.numberOfDimensions_));
  }

  void HookeJeevesAlgorithm::setInitialStepSize(
      const double initialStepSize) {
    if (initialStepSize < 0) {
      throw std::domain_error("HookeJeevesAlgorithm.setInitialStepSize: The initial step size must be positive (including 0).");
    }

    initialStepSize_ = initialStepSize;
  }

  double HookeJeevesAlgorithm::getInitialStepSize() const {
    return initialStepSize_;
  }

  void HookeJeevesAlgorithm::setStepSizeDecrease(
      const double stepSizeDecrease) {
    if (stepSizeDecrease <= 0 || stepSizeDecrease >= 1) {
      throw std::domain_error("HookeJeevesAlgorithm.setStepSizeDecrease: The step size decrease must be within the interval (0, 1).");
    }

    stepSizeDecrease_ = stepSizeDecrease;
  }

  double HookeJeevesAlgorithm::getStepSizeDecrease() const {
    return stepSizeDecrease_;
  }
}
