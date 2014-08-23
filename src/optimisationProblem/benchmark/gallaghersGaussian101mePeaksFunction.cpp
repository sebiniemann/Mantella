#include <optimisationProblem/benchmark/gallaghersGaussian101mePeaksFunction.hpp>

#include <algorithm>
using std::max;

#include <cmath>
using std::sqrt;
using std::pow;
using std::exp;

#include <limits>
using std::numeric_limits;

#include <random>
using std::uniform_int_distribution;

#include <armadillo>
using arma::sqrt;
using arma::abs;
using arma::mean;
using arma::diagmat;
using arma::randu;
using arma::dot;

#include <helper/random.hpp>

namespace hop {
  GallaghersGaussian101mePeaksFunction::GallaghersGaussian101mePeaksFunction(const unsigned int &numberOfDimensions) : BenchmarkProblem(numberOfDimensions), _weight(101) {
    _weight.at(0) = 10.0;
    for(size_t n = 1; n < _weight.n_elem; n++) {
      _weight.at(n) = 1.1 + 8.0 * static_cast<double>(n - 1) / 99.0;
    }
  }

  double GallaghersGaussian101mePeaksFunction::getObjectiveValueImplementation(const Col<double> &parameter) const {
    double maximalValue = numeric_limits<double>::lowest();
    for(unsigned int k = 0; k < 101; k++) {
      Col<double> parameterTransposition = parameter - _localOptimaY101.col(k);
      maximalValue = max(maximalValue, _weight.at(k) * exp(-1.0 / (2.0 * static_cast<double>(_numberOfDimensions)) * dot(parameterTransposition, _rotationR.t() * diagmat(_deltaC101.col(k)) * _rotationR * parameterTransposition)));
    }

    return pow(getOscillationTransformation(10.0 - maximalValue), 2) + getPenality(parameter);
  }
}
