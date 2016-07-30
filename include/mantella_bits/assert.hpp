#pragma once

// C++ standard library
#include <unordered_map>

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/armadillo.hpp"
// IWYU pragma: no_forward_declare mant::Hash
// IWYU pragma: no_forward_declare mant::IsEqual

namespace mant {
  bool isRepresentableAsInteger(
      double value);

  bool isRepresentableAsFloatingPoint(
      arma::uword value);

  bool isRotationMatrix(
      const arma::mat& rotationCandidate);

  bool isPermutationVector(
      const arma::uvec& permutationCandidate,
      const arma::uword numberOfElements,
      const arma::uword cycleSize);
  bool isPermutationVector(
      const arma::uvec& permutationCandidate,
      const arma::uword numberOfElements);

  bool isSymmetric(
      const arma::mat& symmetricCandidate);

  bool isPositiveSemiDefinite(
      const arma::mat& positiveSemiMatrixCandidate);

  bool isDimensionallyConsistent(
      const std::unordered_map<arma::vec, double, Hash, IsEqual>& samples);
}
