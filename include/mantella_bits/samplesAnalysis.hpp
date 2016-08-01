#pragma once

// C++ standard library
#include <unordered_map>
#include <vector>

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/armadillo.hpp"
// IWYU pragma: no_forward_declare mant::Hash
// IWYU pragma: no_forward_declare mant::IsEqual
namespace mant {
  class OptimisationProblem;
}

namespace mant {
  double fitnessDistanceCorrelation(
      const std::unordered_map<arma::vec, double, Hash, IsEqual>& samples);

  double lipschitzContinuity(
      const std::unordered_map<arma::vec, double, Hash, IsEqual>& samples);

  std::vector<arma::uvec> additiveSeparability(
      OptimisationProblem& optimisationProblem,
      const arma::uword numberOfEvaluations,
      const double minimalConfidence);
  std::vector<arma::uvec> additiveSeparability(
      OptimisationProblem& optimisationProblem,
      const arma::uword numberOfEvaluations);
}
