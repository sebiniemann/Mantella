#pragma once

// C++ standard library
#include <vector>

// Armadillo
#include <armadillo>

namespace mant {
  class OptimisationProblem;
}

namespace mant {
  double fitnessDistanceCorrelation(
      arma::mat parameters,
      arma::rowvec objectiveValues);

  double lipschitzContinuity(
      const arma::mat& parameters,
      const arma::rowvec& objectiveValues);

  arma::field<arma::uvec> additiveSeparability(
      OptimisationProblem& optimisationProblem,
      const arma::uword numberOfEvaluations,
      const double minimalConfidence);
  arma::field<arma::uvec> additiveSeparability(
      OptimisationProblem& optimisationProblem,
      const arma::uword numberOfEvaluations);
}
