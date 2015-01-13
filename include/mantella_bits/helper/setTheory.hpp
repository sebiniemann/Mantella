#pragma once

// C++ Standard Library
#include <vector>
#include <utility>

// Armadillo
#include <armadillo>

namespace mant {
  std::vector<std::pair<arma::Col<unsigned int>, arma::Col<unsigned int>>> getTwoSetsPartitions(
      const unsigned int& numberOfElements) noexcept;
}
