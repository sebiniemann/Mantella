#include <optimisationProblem/benchmark/katsuuraFunction.hpp>

#include <algorithm>
using std::min;

#include <cmath>
using std::sqrt;

#include <random>
using std::bernoulli_distribution;

#include <armadillo>
using arma::diagmat;
using arma::sign;
using arma::square;
using arma::accu;
using arma::cos;
using arma::datum;

#include <helper/random.hpp>

namespace hop {
  KatsuuraFunction::KatsuuraFunction(const unsigned int &numberOfDimensions) : BenchmarkProblem(numberOfDimensions), _delta(getScaling(sqrt(100.0))), _rotationR(getRandomRotation()), _rotationQ(getRandomRotation()), _xOpt(numberOfDimensions) {
    _xOpt.fill(bernoulli_distribution(0.5)(Random::RNG) ? 2.5 : -2.5);

    _s = 1.0 - 1.0 / (2.0 * sqrt(static_cast<double>(_numberOfDimensions) + 20.0) - 8.2);
    _mu1 = sqrt((6.25 - 1) / _s);
  }

  double KatsuuraFunction::getObjectiveValueImplementation(const Col<double> &parameter) const {
    Col<double> xHat = 2 * sign(_xOpt) % parameter;
    Col<double> z = _rotationQ * diagmat(_delta) * _rotationR * (xHat - 2.5);

    return min(accu(square(xHat - 2.5)), static_cast<double>(_numberOfDimensions) + _s * accu(square(xHat - _mu1))) + 10.0 * (static_cast<double>(_numberOfDimensions) - accu(cos(2.0 * datum::pi * z))) + 10000 * getPenality(parameter);
  }
}
