#pragma once

// C++ standard library
#include <random>

// Armadillo
#include <armadillo>

namespace mant {
  arma::mat randomRotationMatrix(
      const arma::uword numberOfDimensions);

  arma::uvec randomPermutationVector(
      const arma::uword numberOfElements,
      const arma::uword cycleSize);
  arma::uvec randomPermutationVector(
      const arma::uword numberOfElements);

  arma::vec randomNeighbour(
      const arma::vec& parameter,
      const double minimalDistance,
      const double maximalDistance);
  arma::vec randomNeighbour(
      const arma::vec& parameter,
      const double maximalDistance);

  arma::mat uniformRandomNumbers(
      const arma::uword numberOfRows,
      const arma::uword numberOfColumns,
      std::uniform_real_distribution<double> distribution);
  arma::mat uniformRandomNumbers(
      const arma::uword numberOfRows,
      const arma::uword numberOfColumns);

  arma::vec uniformRandomNumbers(
      const arma::uword numberOfElements,
      std::uniform_real_distribution<double> distribution);
  arma::vec uniformRandomNumbers(
      const arma::uword numberOfElements);

  arma::mat normalRandomNumbers(
      const arma::uword numberOfRows,
      const arma::uword numberOfColumns,
      std::normal_distribution<double> distribution);
  arma::mat normalRandomNumbers(
      const arma::uword numberOfRows,
      const arma::uword numberOfColumns);

  arma::vec normalRandomNumbers(
      const arma::uword numberOfElements,
      std::normal_distribution<double> distribution);
  arma::vec normalRandomNumbers(
      const arma::uword numberOfElements);
}
