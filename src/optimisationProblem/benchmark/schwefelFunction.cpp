#include <optimisationProblem/benchmark/schwefelFunction.hpp>

#include <cmath>
using std::sqrt;

#include <random>
using std::bernoulli_distribution;

#include <armadillo>
using arma::sign;
using arma::sqrt;
using arma::abs;
using arma::mean;
using arma::diagmat;

#include <helper/random.hpp>

namespace hop {
  SchwefelFunction::SchwefelFunction(const unsigned int &numberOfDimensions) : BenchmarkProblem(numberOfDimensions), _delta(getScaling(sqrt(10))), _direction(numberOfDimensions) {
    _direction.fill(bernoulli_distribution(0.5)(Random::RNG) ? 1.0 : -1.0);
  }

  double SchwefelFunction::getObjectiveValueImplementation(const Col<double> &parameter) const {
    Col<double> xOpt = 2.10484373165 * _direction;
    Col<double> xHat = 2 * _direction % parameter;

    Col<double> zHat(xHat.n_elem);
    zHat.at(0) = xHat.at(0);
    zHat.subvec(1, zHat.n_elem - 1) = xHat.subvec(1, zHat.n_elem - 1) + 0.25 * (xHat.subvec(0, xHat.n_elem - 2) - xOpt.subvec(0, xOpt.n_elem - 2));

    Col<double> z = 100 * diagmat(_delta) * (zHat - xOpt) + xOpt;

    return mean(z % sin(sqrt(abs(z)))) + 4.189828872724339 + 100.0 * getPenality(z / 100.0);
  }
}
