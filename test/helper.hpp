#pragma once

// Catch
#include <catch.hpp>

// C++ Standard Library
#include <cstdlib>
#include <vector>

// Armadillo
#include <armadillo>

template <typename ParameterType>
void compare(
    const std::vector<arma::Col<ParameterType>>& actual,
    const arma::Mat<ParameterType>& expected);

template <typename ParameterType>
void compare(
    const arma::Mat<ParameterType>& actual,
    const arma::Mat<ParameterType>& expected);

template <typename ParameterType>
void compare(
    const arma::Col<ParameterType>& actual,
    const arma::Col<ParameterType>& expected);

template <typename ParameterType>
void compare(
    const std::vector<arma::Col<ParameterType>>& actual,
    const arma::Mat<ParameterType>& expected) {
  REQUIRE(actual.size() == expected.n_cols);

  for(std::size_t n = 0; n < expected.n_cols; ++n) {
    compare<ParameterType>(actual.at(n), expected.col(n));
  }
}

template <typename ParameterType>
void compare(
    const arma::Mat<ParameterType>& actual,
    const arma::Mat<ParameterType>& expected) {
  REQUIRE(actual.n_cols == expected.n_cols);

  for(std::size_t n = 0; n < expected.n_cols; ++n) {
    compare<ParameterType>(static_cast<arma::Col<ParameterType>>(actual.col(n)), static_cast<arma::Col<ParameterType>>(expected.col(n)));
  }
}

template <typename ParameterType>
void compare(
    const arma::Col<ParameterType>& actual,
    const arma::Col<ParameterType>& expected) {
  REQUIRE(actual.n_elem == expected.n_elem);

  for (std::size_t k = 0; k < expected.n_elem; ++k) {
    if(std::isfinite(expected.at(k))) {
      CHECK(actual.at(k) == Approx(expected.at(k)));
    } else {
      CHECK(actual.at(k) == expected.at(k));
    }
  }
}
