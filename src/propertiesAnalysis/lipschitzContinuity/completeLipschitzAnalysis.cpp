#include <hop_bits/propertiesAnalysis/lipschitzContinuity/completeLipschitzAnalysis.hpp>

// C++ Standard Library
#include <cstdlib>
#include <algorithm>

namespace hop {
  void CompleteLipschitzAnalysis::analyseImplementation(
      const arma::Mat<double>& parameters,
      const arma::Col<double>& objectiveValues) {
    double lipschitzConstant = 0;

    for (std::size_t n = 0; n < parameters.n_cols; ++n) {
      arma::Col<double> parameter = parameters.col(n);
      double objectiveValue = objectiveValues.at(n);
      for (std::size_t k = n + 1; k < parameters.n_cols; ++k) {
        lipschitzConstant = std::max(lipschitzConstant, std::abs((objectiveValues.at(k) - objectiveValue) / arma::norm(parameters.col(k) - parameter)));
      }
    }

    lipschitzConstant_ = lipschitzConstant;
  }
}
