#include <optimisationProblem/benchmark/rosenbrockFunction.hpp>

#include <algorithm>
using std::max;

#include <armadillo>
using arma::accu;
using arma::square;

namespace hop {
  RosenbrockFunction::RosenbrockFunction(const unsigned int &numberOfDimensions) : BenchmarkProblem(numberOfDimensions), _max(max(1.0, sqrt(static_cast<double>(numberOfDimensions) / 8.0))) {
    _translation *= 0.75;
  }

  double RosenbrockFunction::getObjectiveValueImplementation(const Col<double> &parameter) const {
    Col<double> z = _max * (parameter - _translation) + 1.0;

    return accu(100.0 * square(square(z.subvec(0, z.n_elem - 2)) - z.subvec(1, z.n_elem - 1)) + square(z.subvec(0, z.n_elem - 2) - 1.0));
  }
}
