#include <optimisationProblem/benchmark/rosenbrockFunction.hpp>

#include <algorithm>
using std::max;

#include <armadillo>
using arma::accu;
using arma::square;

namespace hop {
  RosenbrockFunction::RosenbrockFunction(const unsigned int &numberOfDimensions) : BenchmarkProblem(numberOfDimensions), _max(max(1.0, sqrt(static_cast<double>(numberOfDimensions)) / 8.0)) {

  }

  void RosenbrockFunction::setTranslation(const Col<double>& translation) {
    _translation = 0.75 * translation;
  }

  double RosenbrockFunction::getObjectiveValueImplementation(const Col<double> &parameter) const {
    Col<double> z = _max * (parameter - _translation) + 1.0;

    return 100.0 * accu(square(square(z.subvec(0, z.n_elem - 2)) - z.subvec(1, z.n_elem - 1))) + accu(square(z.subvec(0, z.n_elem - 2) - 1.0));
  }
}
