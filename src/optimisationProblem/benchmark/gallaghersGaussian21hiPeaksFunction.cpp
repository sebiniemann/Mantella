#include <hop_bits/optimisationProblem/benchmark/gallaghersGaussian21hiPeaksFunction.hpp>

#include <algorithm>
#include <cmath>
#include <limits>

namespace hop {
  GallaghersGaussian21hiPeaksFunction::GallaghersGaussian21hiPeaksFunction(const unsigned int& numberOfDimensions)
    : BenchmarkProblem(numberOfDimensions),
      weight_(21) {
    weight_.at(0) = 10.0;
    for(std::size_t n = 1; n < weight_.n_elem; ++n) {
      weight_.at(n) = 1.1 + 8.0 * static_cast<double>(n - 1) / 19.0;
    }
  }

  double GallaghersGaussian21hiPeaksFunction::getObjectiveValueImplementation(const arma::Col<double>& parameter) const {
    double maximalValue = std::numeric_limits<double>::lowest();
    for(unsigned int k = 0; k < 21; ++k) {
      arma::Col<double> parameterTransposition = parameter - localOptimaY21_.col(k);
      maximalValue = std::max(maximalValue, weight_.at(k) * std::exp(-1.0 / (2.0 * static_cast<double>(numberOfDimensions_)) * arma::dot(parameterTransposition, rotationR_.t() * arma::diagmat(deltaC21_.col(k)) * rotationR_ * parameterTransposition)));
    }

    return std::pow(getOscillationTransformation(10.0 - maximalValue), 2) + getPenality(parameter);
  }

  std::string GallaghersGaussian21hiPeaksFunction::to_string() const {
    return "GallaghersGaussian21hiPeaksFunction";
  }
}
