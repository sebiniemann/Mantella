#pragma once

// C++ standard library
#include <functional>
#include <unordered_map>
#include <utility>

// Armadillo
namespace arma {
  template <typename T>
  class Col;
}

// Mantella
#include "mantella_bits/armadillo.hpp"
// IWYU pragma: no_forward_declare mant::Hash
// IWYU pragma: no_forward_declare mant::IsEqual

namespace mant {
  double continuity(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples,
      const std::function<double(const std::pair<arma::Col<double>, double>& firstSample, const std::pair<arma::Col<double>, double>& secondSample)> continuityFunction);

  double lipschitzContinuity(
      const std::pair<arma::Col<double>, double>& firstSample,
      const std::pair<arma::Col<double>, double>& secondSample);

  double alphaHoelderContinuity(
      const std::pair<arma::Col<double>, double>& firstSample,
      const std::pair<arma::Col<double>, double>& secondSample,
      const double alpha);
}
