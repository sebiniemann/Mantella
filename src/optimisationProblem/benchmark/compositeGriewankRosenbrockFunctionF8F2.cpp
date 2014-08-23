#include <optimisationProblem/benchmark/compositeGriewankRosenbrockFunctionF8F2.hpp>

#include <algorithm>
using std::max;

#include <cmath>
using std::sqrt;

#include <armadillo>
using arma::square;
using arma::accu;
using arma::cos;
using arma::mean;

namespace hop {
  CompositeGriewankRosenbrockFunctionF8F2::CompositeGriewankRosenbrockFunctionF8F2(const unsigned int &numberOfDimensions) : BenchmarkProblem(numberOfDimensions), _max(max(1.0, sqrt(static_cast<double>(numberOfDimensions)) / 8.0)) {

  }

  double CompositeGriewankRosenbrockFunctionF8F2::getObjectiveValueImplementation(const Col<double> &parameter) const {
    Col<double> z = _max * _rotationR * parameter + 0.5;
    Col<double> s = 100.0 * square(square(z.subvec(0, z.n_elem - 2)) - z.subvec(1, z.n_elem - 1)) + square(1.0 - z.subvec(0, z.n_elem - 2));

    return 10 * mean(s / 4000.0 - cos(s)) + 10;
  }
}
