#include <hop_bits/optimisationProblem/benchmark/gallaghersGaussian21hiPeaksFunction.hpp>

#include <algorithm>
#include <cmath>
#include <limits>

namespace hop {
  GallaghersGaussian21hiPeaksFunction::GallaghersGaussian21hiPeaksFunction(const unsigned int &numberOfDimensions) : BenchmarkProblem(numberOfDimensions), _weight(21) {
    _weight.at(0) = 10.0;
    for(std::size_t n = 1; n < _weight.n_elem; n++) {
      _weight.at(n) = 1.1 + 8.0 * static_cast<double>(n - 1) / 19.0;
    }
  }

  double GallaghersGaussian21hiPeaksFunction::getObjectiveValueImplementation(const arma::Col<double> &parameter) const {
    double maximalValue = std::numeric_limits<double>::lowest();
    for(unsigned int k = 0; k < 21; k++) {
      arma::Col<double> parameterTransposition = parameter - _localOptimaY21.col(k);
      maximalValue = std::max(maximalValue, _weight.at(k) * std::exp(-1.0 / (2.0 * static_cast<double>(_numberOfDimensions)) * arma::dot(parameterTransposition, _rotationR.t() * arma::diagmat(_deltaC21.col(k)) * _rotationR * parameterTransposition)));
    }

    return std::pow(getOscillationTransformation(10.0 - maximalValue), 2) + getPenality(parameter);
  }
}
