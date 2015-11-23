#pragma once

// C++ standard library
#include <string>
#include <unordered_map>

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/armadillo.hpp"
// IWYU pragma: no_forward_declare mant::Hash
// IWYU pragma: no_forward_declare mant::IsEqual

namespace mant {
  void verify(
      const bool expression,
      const std::string& errorMessage);

  bool isRotationMatrix(
      const arma::Mat<double>& rotationCandidate);

  bool isPermutationVector(
      const arma::Col<arma::uword>& permutationCandidate,
      const arma::uword numberOfElements,
      const arma::uword cycleSize);

  bool isSymmetric(
      const arma::Mat<double>& symmetricCandidate);

  bool isPositiveSemiDefinite(
      const arma::Mat<double>& positiveSemiMatrixCandidate);

  bool isDimensionallyConsistent(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples);
}
