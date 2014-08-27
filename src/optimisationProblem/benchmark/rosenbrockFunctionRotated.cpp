#include <optimisationProblem/benchmark/rosenbrockFunctionRotated.hpp>

#include <algorithm>
#include <cmath>

namespace hop {
  RosenbrockFunctionRotated::RosenbrockFunctionRotated(const unsigned int &numberOfDimensions) : BenchmarkProblem(numberOfDimensions), _max(std::max(1.0, std::sqrt(static_cast<double>(numberOfDimensions)) / 8.0)) {

  }

  double RosenbrockFunctionRotated::getObjectiveValueImplementation(const arma::Col<double> &parameter) const {
    arma::Col<double> z = _max * _rotationR * parameter + 0.5;

    return 100.0 * arma::accu(arma::square(arma::square(z.subvec(0, z.n_elem - 2)) - z.subvec(1, z.n_elem - 1))) + arma::accu(arma::square(z.subvec(0, z.n_elem - 2) - 1.0));
  }
}
