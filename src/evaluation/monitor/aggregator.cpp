#include <hop_bits/evaluation/monitor/aggregator.hpp>

namespace hop {
  double Aggregator::min(const arma::Row<double>& values) {
    return values.min();
  }

  arma::Col<double> Aggregator::min(const arma::Mat<double>& values) {
    return arma::min(values, 1);
  }

  double Aggregator::max(const arma::Row<double>& values) {
    return values.max();
  }

  arma::Col<double> Aggregator::max(const arma::Mat<double>& values) {
    return arma::max(values, 1);
  }


  double Aggregator::mean(const arma::Row<double>& values) {
    return arma::mean(values);
  }

  arma::Col<double> Aggregator::mean(const arma::Mat<double>& values) {
    return arma::mean(values, 1);
  }


  double Aggregator::variance(const arma::Row<double>& values) {
    return arma::var(values);
  }

  arma::Col<double> Aggregator::variance(const arma::Mat<double>& values) {
    return arma::var(values, 0, 1);
  }


  double Aggregator::standardDeviation(const arma::Row<double>& values) {
    if(values.n_elem < 2) {
      //TODO Add exception
    }

    return arma::stddev(values);
  }

  arma::Col<double> Aggregator::standardDeviation(const arma::Mat<double>& values) {
    if(values.n_rows < 2) {
      //TODO Add exception
    }

    return arma::stddev(values, 0, 1);
  }


  arma::Row<double> Aggregator::differences(const arma::Row<double>& values) {
    if(values.n_elem < 2) {
      //TODO Add exception
    }

    return values.subvec(0, values.n_elem - 2) - values.subvec(1, values.n_elem - 1);
  }

  arma::Mat<double> Aggregator::differences(const arma::Mat<double>& values) {
    if(values.n_rows < 2) {
      //TODO Add exception
    }

    return values.rows(0, values.n_rows - 2) - values.rows(1, values.n_rows - 1);
  }
}
