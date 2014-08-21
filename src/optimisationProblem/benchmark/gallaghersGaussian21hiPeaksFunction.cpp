#include <optimisationProblem/benchmark/gallaghersGaussian21hiPeaksFunction.hpp>

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
  GallaghersGaussian21hiPeaksFunction::GallaghersGaussian21hiPeaksFunction(const unsigned int &numberOfDimensions) : BenchmarkProblem(numberOfDimensions), _rotationR(getRandomRotation()), _delta21(numberOfDimensions, 21), _localOptima21(numberOfDimensions, 21), _weight(21) {
    _delta21.col(0) = getScaling(sqrt(1000.0)) / pow(1000000.0, 0.25);

    uniform_int_distribution<int> uniformIntDistribution(0, 19);
    for(size_t j = 1; j < _delta21.n_cols; j++) {
      _delta21.col(j) = getScaling(sqrt(1000.0)) / pow(pow(1000.0, 2.0 * static_cast<double>(uniformIntDistribution(Random::RNG)) / 19.0), 0.25);
    }

    _localOptima21.col(0) = randu(numberOfDimensions) * 7.84 - 3.92;
    _localOptima21.cols(1, _localOptima21.n_cols - 1) = randu(numberOfDimensions, _localOptima21.n_cols - 1) * 9.8 - 4.9;

    _weight.at(0) = 10.0;
    for(size_t n = 1; n < _weight.n_elem; n++) {
      _weight.at(n) = 1.1 + 8.0 * static_cast<double>(n - 1) / 19.0;
    }
  }

  double GallaghersGaussian21hiPeaksFunction::getObjectiveValueImplementation(const Col<double> &parameter) const {
    double maximalValue = numeric_limits<double>::max();
    for(unsigned int k = 0; k < 21; k++) {
      Col<double> parameterTransposition = parameter - _localOptima21.col(k);
      maximalValue = max(maximalValue, _weight.at(k) * exp(-1.0 / (2.0 * static_cast<double>(_numberOfDimensions)) * dot(parameterTransposition, _rotationR.t() * diagmat(_delta21.col(k)) * _rotationR * parameterTransposition)));
    }

    return pow(getOscillationTransformation(10.0 - maximalValue), 2) + getPenality(parameter);
  }
}
