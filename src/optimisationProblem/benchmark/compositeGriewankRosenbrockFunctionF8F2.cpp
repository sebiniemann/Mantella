#include <optimisationProblem/benchmark/compositeGriewankRosenbrockFunctionF8F2.hpp>

#include <algorithm>
#include <cmath>

namespace hop {
  CompositeGriewankRosenbrockFunctionF8F2::CompositeGriewankRosenbrockFunctionF8F2(const unsigned int &numberOfDimensions) : BenchmarkProblem(numberOfDimensions), _max(std::max(1.0, std::sqrt(static_cast<double>(numberOfDimensions)) / 8.0)) {

  }

  double CompositeGriewankRosenbrockFunctionF8F2::getObjectiveValueImplementation(const arma::Col<double> &parameter) const {
    arma::Col<double> z = _max * _rotationR * parameter + 0.5;
    arma::Col<double> s = 100.0 * arma::square(arma::square(z.subvec(0, z.n_elem - 2)) - z.subvec(1, z.n_elem - 1)) + arma::square(1.0 - z.subvec(0, z.n_elem - 2));

    return 10 * arma::mean(s / 4000.0 - arma::cos(s)) + 10;
  }
}
