#pragma once

// Catch
#include <catch.hpp>

// C++ standard library
#include <vector>

// Armadillo
#include <armadillo>

arma::uword getRandomNumberOfValues();
    
arma::uword getRandomNumberOfValues(
    const arma::uword minimalNumberOfDimensions);
    
arma::uword getDifferentRandomNumberOfValues(
    const arma::uword numberOfDimensions);
    
arma::Col<double> getRandomValues(
    const arma::uword numberOfDimensions);
    
arma::Mat<double> getRandomValues(
    const arma::uword numberOfDimensions,
    const arma::uword numberOfSamples);

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

