#include <optimisationProblem/benchmark/rosenbrockFunction.hpp>

#include <algorithm>
#include <cmath>

namespace hop {
  RosenbrockFunction::RosenbrockFunction(const unsigned int &numberOfDimensions) : BenchmarkProblem(numberOfDimensions), _max(std::max(1.0, std::sqrt(static_cast<double>(numberOfDimensions)) / 8.0)) {

  }

  void RosenbrockFunction::setTranslation(const arma::Col<double>& translation) {
    _translation = 0.75 * translation;
  }

  double RosenbrockFunction::getObjectiveValueImplementation(const arma::Col<double> &parameter) const {
    arma::Col<double> z = _max * (parameter - _translation) + 1.0;

    return 100.0 * arma::accu(arma::square(arma::square(z.subvec(0, z.n_elem - 2)) - z.subvec(1, z.n_elem - 1))) + arma::accu(arma::square(z.subvec(0, z.n_elem - 2) - 1.0));
  }
}
