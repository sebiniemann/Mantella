#pragma once

// Catch
#include <catch.hpp>

// C++ standard library
#include <cstdlib>
#include <vector>

// Armadillo
#include <armadillo>

void COMPARE_SET(
    const std::vector<std::pair<arma::Col<double>, double>>& actual,
    const std::vector<arma::Col<double>>& expected);

void COMPARE(
    const arma::Col<double>& actual,
    const arma::Col<double>& expected);

void COMPARE(
    const arma::Col<arma::uword>& actual,
    const arma::Col<arma::uword>& expected);

void COMPARE(
    const arma::Mat<double>& actual,
    const arma::Mat<double>& expected);
