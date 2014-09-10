#include <hop_bits/optimisationProblem/benchmark/rosenbrockFunction.hpp>

#include <algorithm>
#include <cmath>

namespace hop {
  void RosenbrockFunction::setTranslation(const arma::Col<double>& translation) {
    translation_ = 0.75 * translation;
  }

  double RosenbrockFunction::getObjectiveValueImplementation(const arma::Col<double>& parameter) const {
    arma::Col<double> z = max_ * (parameter - translation_) + 1.0;

    return 100.0 * arma::accu(arma::square(arma::square(z.subvec(0, z.n_elem - 2)) - z.subvec(1, z.n_elem - 1))) + arma::accu(arma::square(z.subvec(0, z.n_elem - 2) - 1.0));
  }
}
