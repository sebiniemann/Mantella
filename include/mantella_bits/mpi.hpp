#pragma once

// Armadillo
#include <armadillo>

namespace mant {
  arma::mat synchronise(
      arma::mat data);

  arma::umat synchronise(
      const arma::umat& data);

  arma::vec synchronise(
      arma::vec data);

  arma::uvec synchronise(
      const arma::uvec& data);

  arma::rowvec synchronise(
      arma::rowvec data);

  arma::urowvec synchronise(
      const arma::urowvec& data);

  double synchronise(
      double data);

  arma::uword synchronise(
      const arma::uword data);
}
