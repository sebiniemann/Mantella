#pragma once

// C++ standard library
#include <functional>
#include <unordered_map>

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/armadillo.hpp"
// IWYU pragma: no_forward_declare mant::Hash
// IWYU pragma: no_forward_declare mant::IsEqual

namespace mant {
  std::unordered_map<arma::Col<double>, double, Hash, IsEqual> randomly(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples,
      const arma::uword numberOfSamplesToSelect);

  std::unordered_map<arma::Col<double>, double, Hash, IsEqual> bestFitting(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples,
      const arma::uword numberOfSamplesToSelect,
      const std::function<arma::Col<double>(const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>&)> propertyAnalysis);

  std::unordered_map<arma::Col<double>, double, Hash, IsEqual> bestFitting(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples,
      const arma::uword numberOfSamplesToSelect,
      const std::function<double(const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>&)> propertyAnalysis);

  std::unordered_map<arma::Col<double>, double, Hash, IsEqual> nearestNeighbours(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples,
      const arma::uword numberOfSamplesToSelect,
      const arma::Col<double>& parameter,
      const std::function<double(const arma::Col<double>&, const arma::Col<double>&)> distanceFunction);

  std::unordered_map<arma::Col<double>, double, Hash, IsEqual> fixedRadiusNearestNeighbours(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples,
      const double radius,
      const arma::Col<double>& parameter,
      const std::function<double(const arma::Col<double>&, const arma::Col<double>&)> distanceFunction);

  std::unordered_map<arma::Col<double>, double, Hash, IsEqual> elitists(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples,
      const arma::uword numberOfSamplesToSelect);
}
