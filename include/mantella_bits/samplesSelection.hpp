#pragma once

// C++ standard library
#include <unordered_map>

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/armadillo.hpp"
// IWYU pragma: no_forward_declare mant::Hash
// IWYU pragma: no_forward_declare mant::IsEqual

namespace mant {
  std::unordered_map<arma::vec, double, Hash, IsEqual> elitists(
      const std::unordered_map<arma::vec, double, Hash, IsEqual>& samples,
      const arma::uword numberOfSamplesToSelect);
}
