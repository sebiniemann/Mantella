#include "mantella_bits/samplesAnalysis.hpp"

// C++ standard library
#include <algorithm>
#include <cmath>
#include <utility>

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/assert.hpp"

namespace mant {
  double fitnessDistanceCorrelation(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples) {
    verify(samples.size() > 1, "fitnessDistanceCorrelation: The number of samples must be at least 2.");
    verify(isDimensionallyConsistent(samples), ""); // TODO

    arma::Mat<double> parameters(samples.cbegin()->first.n_elem, samples.size());
    arma::Row<double> objectiveValues(parameters.n_cols);

    arma::uword n = 0;
    for (const auto& sample : samples) {
      parameters.col(n) = sample.first;
      objectiveValues(n) = sample.second;
      ++n;
    }

    arma::uword bestParameterIndex;
    objectiveValues.min(bestParameterIndex);
    parameters.each_col() -= parameters.col(bestParameterIndex);
    parameters.shed_col(bestParameterIndex);
    objectiveValues.shed_col(bestParameterIndex);

    return arma::as_scalar(arma::cor(arma::sqrt(arma::sum(arma::square(parameters))), objectiveValues));
  }

  double lipschitzContinuity(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples) {
    verify(samples.size() > 1, ""); // TODO
    verify(isDimensionallyConsistent(samples), ""); // TODO

    double continuity = 0.0;
    for (auto firstSample = samples.cbegin(); firstSample != samples.cend();) {
      for (auto secondSample = ++firstSample; secondSample != samples.cend(); ++secondSample) {
        continuity = std::max(continuity, std::abs(firstSample->second - secondSample->second) / arma::norm(firstSample->first - secondSample->first));
      }
    }

    return continuity;
  }
}
