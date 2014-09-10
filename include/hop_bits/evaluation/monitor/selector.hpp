#pragma once

#include <armadillo>

namespace hop {
  class Selector {
    public:
      Selector() = delete;
      Selector(const Selector&) = delete;
      Selector& operator=(const Selector&) = delete;

      static arma::Row<double> increasing(const arma::Row<double>& values);
      static arma::Mat<double> increasing(const arma::Mat<double>& values, const arma::Row<double>& indicator);

      static arma::Row<double> decreasing(const arma::Row<double>& values);
      static arma::Mat<double> decreasing(const arma::Mat<double>& values, const arma::Row<double>& indicator);

      static arma::Row<double> subset(const arma::Row<double>& values, const arma::Row<arma::uword>& indicator);
      static arma::Mat<double> subset(const arma::Mat<double>& values, const arma::Row<arma::uword>& indicator);
  };
}
