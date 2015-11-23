#pragma once

// C++ standard library
#include <vector>
#include <utility>

// Armadillo
#include <armadillo>

namespace mant {
  arma::Mat<double> hammersleySet(
      const arma::Col<arma::uword>& base,
      const arma::Col<arma::uword>& seed,
      const arma::uword numberOfColumms);

  arma::Mat<double> haltonSequence(
      const arma::Col<arma::uword>& base,
      const arma::Col<arma::uword>& seed,
      const arma::uword numberOfColumms);

  arma::Col<double> vanDerCorputSequence(
      const arma::uword base,
      const arma::uword seed,
      const arma::uword numberOfColumms);

  std::vector<std::pair<arma::Col<arma::uword>, arma::Col<arma::uword>>> twoSetsPartitions(
      const arma::uword numberOfElements);
}
