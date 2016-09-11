#pragma once

// Armadillo
#include <armadillo>

// C++ standard library
#include <functional>

namespace mant {
  double brent(
      std::function<double(
          double)> objectiveFunction,
      double lowerBound,
      double upperBound,
      arma::uword maximalNumberOfIterations);
}
