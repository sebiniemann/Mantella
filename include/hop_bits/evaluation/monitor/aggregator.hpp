#pragma once

#include <armadillo>

namespace hop {
  class Aggregator {
    public:
      Aggregator() = delete;
      Aggregator(const Aggregator&) = delete;
      Aggregator& operator=(const Aggregator&) = delete;

      static double min(const arma::Row<double>& values);
      static arma::Col<double> min(const arma::Mat<double>& values);

      static double max(const arma::Row<double>& values);
      static arma::Col<double> max(const arma::Mat<double>& values);

      static double mean(const arma::Row<double>& values);
      static arma::Col<double> mean(const arma::Mat<double>& values);

      static double variance(const arma::Row<double>& values);
      static arma::Col<double> variance(const arma::Mat<double>& values);

      static double standardDeviation(const arma::Row<double>& values);
      static arma::Col<double> standardDeviation(const arma::Mat<double>& values);

      static arma::Row<double> differences(const arma::Row<double>& values);
      static arma::Mat<double> differences(const arma::Mat<double>& values);
  };
}
