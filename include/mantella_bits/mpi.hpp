#pragma once

// Armadillo
#include <armadillo>

namespace mant {
  arma::mat synchronise(
      arma::mat data);

  arma::vec synchronise(
      arma::vec data);

  arma::uvec synchronise(
      const arma::uvec& data);

  double synchronise(
      double data);

  arma::uword synchronise(
      const arma::uword data);
}
