#pragma once

// Catch
#include <catch.hpp>

// C++ Standard Library
#include <cstdlib>
#include <vector>

// Armadillo
#include <armadillo>

template <typename T>
void compare(
    const arma::Col<T>& actual,
    const arma::Col<T>& expected);

template <typename T>
void compare(
    const arma::Mat<T>& actual,
    const arma::Mat<T>& expected);

template <typename T>
void compare(
    const std::vector<arma::Col<T>>& actual,
    const arma::Mat<T>& expected);
    
//
// Implementation
//

template <typename T>
void compare(
    const arma::Col<T>& actual,
    const arma::Col<T>& expected) {
  REQUIRE(actual.n_elem == expected.n_elem);

  for (std::size_t k = 0; k < expected.n_elem; ++k) {
    if(std::isfinite(expected.at(k))) {
      CHECK(actual.at(k) == Approx(expected.at(k)));
    } else {
      CHECK(actual.at(k) == expected.at(k));
    }
  }
}

template <typename T>
void compare(
    const arma::Mat<T>& actual,
    const arma::Mat<T>& expected) {
  REQUIRE(actual.n_cols == expected.n_cols);

  for(std::size_t n = 0; n < expected.n_cols; ++n) {
    compare<T>(static_cast<arma::Col<T>>(actual.col(n)), static_cast<arma::Col<T>>(expected.col(n)));
  }
}

template <typename T>
void compare(
    const std::vector<arma::Col<T>>& actual,
    const arma::Mat<T>& expected) {
  REQUIRE(actual.size() == expected.n_cols);

  for(std::size_t n = 0; n < expected.n_cols; ++n) {
    compare<T>(actual.at(n), expected.col(n));
  }
}