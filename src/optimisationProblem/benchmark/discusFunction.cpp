#include <hop_bits/optimisationProblem/benchmark/discusFunction.hpp>

namespace hop {
  double DiscusFunction::getObjectiveValueImplementation(const arma::Col<double>& parameter) const {
    arma::Col<double> z = arma::square(getOscillationTransformation(rotationR_ * (parameter - translation_)));
    return 1000000 * z.at(0) + arma::accu(z.subvec(1, z.n_elem - 1));
  }
}
