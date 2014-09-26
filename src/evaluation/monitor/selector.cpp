#include <hop_bits/evaluation/monitor/selector.hpp>

#include <cstdlib>

namespace hop {
  arma::Row<double> Selector::increasing(const arma::Row<double>& values) {
    arma::Row<double> increasing;
    increasing.copy_size(values);

    double maximalValue = values.at(0);
    increasing.at(0) = maximalValue;

    std::size_t numberOfElements = 1;
    for(std::size_t n = 1; n < values.n_elem; ++n) {
      double value = values.at(n);

      if(value >= maximalValue) {
        increasing.at(numberOfElements++) = value;
        maximalValue = value;
      }
    }
    increasing.resize(numberOfElements);

    return increasing;
  }

  arma::Mat<double> Selector::increasing(const arma::Mat<double>& values, const arma::Row<double>& indicator) {
    if(values.n_rows != indicator.n_elem) {
      //TODO Add exception
    }

    arma::Mat<double> increasing;
    increasing.copy_size(values);

    double maximalIndicatorValue = indicator.at(0);
    increasing.at(0) = values.at(0);

    std::size_t numberOfRows = 1;
    for(std::size_t n = 1; n < values.n_rows; ++n) {
      double indicatorValue = indicator.at(n);

      if(indicatorValue >= maximalIndicatorValue) {
        increasing.row(numberOfRows++) = values.row(0);
        maximalIndicatorValue = indicatorValue;
      }
    }
    increasing.resize(numberOfRows, increasing.n_cols);

    return increasing;
  }

  arma::Row<double> Selector::decreasing(const arma::Row<double>& values) {
    arma::Row<double> decreasing;
    decreasing.copy_size(values);

    double minimalValue = values.at(0);
    decreasing.at(0) = minimalValue;

    size_t size = 1;
    for(int n = 1; n < values.n_elem; ++n) {
      double value = values.at(n);

      if(value <= minimalValue) {
        decreasing.at(size++) = value;
        minimalValue = value;
      }
    }
    decreasing.resize(size);

    return decreasing;
  }

  arma::Mat<double> Selector::decreasing(const arma::Mat<double>& values, const arma::Row<double>& indicator) {
    if(values.n_rows != indicator.n_elem) {
      //TODO Add exception
    }

    arma::Mat<double> decreasing;
    decreasing.copy_size(values);

    double minimalIndicatorValue = indicator.at(0);
    decreasing.at(0) = values.at(0);

    std::size_t numberOfRows = 1;
    for(std::size_t n = 1; n < values.n_rows; ++n) {
      double indicatorValue = indicator.at(n);

      if(indicatorValue <= minimalIndicatorValue) {
        decreasing.row(numberOfRows++) = values.row(0);
        minimalIndicatorValue = indicatorValue;
      }
    }
    decreasing.resize(numberOfRows, decreasing.n_cols);

    return decreasing;
  }

  arma::Row<double> Selector::subset(const arma::Row<double>& values, const arma::Row<arma::uword>& indicator) {
    if(values.n_elem != indicator.n_elem) {
      //TODO Add exception
    }

    return values.elem(indicator);
  }

  arma::Mat<double> Selector::subset(const arma::Mat<double>& values, const arma::Row<arma::uword>& indicator) {
    if(values.n_rows != indicator.n_elem) {
      //TODO Add exception
    }

    return values.rows(indicator);
  }
}
