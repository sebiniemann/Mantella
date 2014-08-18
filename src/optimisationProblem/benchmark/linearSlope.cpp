#include <optimisationProblem/benchmark/linearSlope.hpp>

#include <cmath>
using std::sqrt;
using std::pow;

#include <random>
using std::bernoulli_distribution;

#include <armadillo>
using arma::sign;
using arma::find;
using arma::accu;
using arma::abs;
using arma::dot;

#include <helper/random.hpp>

namespace hop {
  LinearSlope::LinearSlope(const unsigned int &numberOfDimensions) : BenchmarkProblem(numberOfDimensions), _scaling(getScaling(10)), _direction(numberOfDimensions) {
    _direction.fill(bernoulli_distribution(0.5)(Random::RNG) ? 5.0 : -5.0);
    _scaling %= sign(_direction);
    _partiallyObjectiveValue = 5 * accu(abs(_scaling));
  }

  double LinearSlope::getObjectiveValueImplementation(const Col<double> &parameter) {
    Col<double> z(parameter);

    Col<uword> outOfBound = find(_direction % z < 25);
    z.elem(outOfBound) = _direction.elem(outOfBound);

    return _partiallyObjectiveValue - dot(_scaling, z);
  }
}
