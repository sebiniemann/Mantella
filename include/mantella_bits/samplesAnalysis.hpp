#pragma once

// C++ standard library
#include <unordered_map>

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
  double fitnessDistanceCorrelation(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples);

  double lipschitzContinuity(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples);
}
