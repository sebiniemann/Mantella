#include "mantella_bits/samplesAnalysis.hpp"

// C++ standard library
#include <algorithm>
#include <cmath>

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/assert.hpp"

namespace mant {
  double continuity(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples,
      const std::function<double(const std::pair<arma::Col<double>, double>& firstSample, const std::pair<arma::Col<double>, double>& secondSample)> continuityFunction) {
    verify(static_cast<bool>(continuityFunction), ""); // TODO
    verify(isDimensionallyConsistent(samples), ""); // TODO

    double continuity = 0.0;
    for (auto firstSample = samples.cbegin(); firstSample != samples.cend();) {
      for (auto secondSample = ++firstSample; secondSample != samples.cend(); ++secondSample) {
        continuity = std::max(continuity, continuityFunction({firstSample->first, firstSample->second}, {secondSample->first, secondSample->second}));
      }
    }

    return continuity;
  }

  double lipschitzContinuity(
      const std::pair<arma::Col<double>, double>& firstSample,
      const std::pair<arma::Col<double>, double>& secondSample) {
    verify(firstSample.first.n_elem == secondSample.first.n_elem, ""); // TODO

    return std::abs(firstSample.second - secondSample.second) / arma::norm(firstSample.first - secondSample.first);
  }

  double alphaHoelderContinuity(
      const std::pair<arma::Col<double>, double>& firstSample,
      const std::pair<arma::Col<double>, double>& secondSample,
      const double alpha) {
    verify(firstSample.first.n_elem == secondSample.first.n_elem, ""); // TODO

    if (alpha <= 1.0) {
      return std::abs(firstSample.second - secondSample.second) / std::pow(arma::norm(firstSample.first - secondSample.first), alpha);
    } else {
      return 1.0;
    }
  }
}
