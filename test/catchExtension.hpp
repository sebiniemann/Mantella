#pragma once

// Catch
#include <catch.hpp>

// C++ standard library
#include <vector>
#include <string>

// Armadillo
#include <armadillo>

extern std::string testDirectory;
  
arma::uword getDiscreteRandomNumber();

arma::uword getDifferentDiscreteRandomNumber(
    const arma::uword discreteRandomNumber);

double getContinuousRandomNumber();

arma::Col<arma::uword> getDiscreteRandomNumbers(
    const arma::uword numberOfDimensions);

arma::Col<double> getContinuousRandomNumbers(
    const arma::uword numberOfDimensions);

arma::Mat<arma::uword> getDiscreteRandomNumbers(
    const arma::uword numberOfRows,
    const arma::uword numberOfColumns);

arma::Mat<double> getContinuousRandomNumbers(
    const arma::uword numberOfRows,
    const arma::uword numberOfColumns);

void HAS_SAME_PARAMETERS(
    const std::vector<arma::Col<arma::uword>>& actualParameters,
    const std::vector<arma::Col<arma::uword>>& expectedParameters);

void HAS_SAME_PARAMETERS(
    const std::vector<std::pair<arma::Col<double>, double>>& samples,
    const std::vector<arma::Col<double>>& parameters);

void IS_EQUAL(
    const arma::Col<double>& actual,
    const arma::Col<double>& expected);

void IS_EQUAL(
    const arma::Col<arma::uword>& actual,
    const arma::Col<arma::uword>& expected);

void IS_EQUAL(
    const arma::Mat<double>& actual,
    const arma::Mat<double>& expected);

void IS_UNIFORM(
    const arma::Col<double>& data,
    const double lowerBound,
    const double upperBound);

void IS_UNIFORM(
    const arma::Col<arma::uword>& data,
    const arma::uword lowerBound,
    const arma::uword upperBound);
