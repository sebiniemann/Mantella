#include <hop_bits/optimisationProblem/benchmark/ellipsoidalFunction.hpp>

namespace hop {
  double EllipsoidalFunction::getObjectiveValueImplementation(const arma::Col<double>& parameter) const {
    return arma::dot(scaling_, arma::square(getOscillationTransformation(parameter - translation_)));
  }
}
