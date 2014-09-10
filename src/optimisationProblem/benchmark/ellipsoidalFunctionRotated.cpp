#include <hop_bits/optimisationProblem/benchmark/ellipsoidalFunctionRotated.hpp>

namespace hop {
  double EllipsoidalFunctionRotated::getObjectiveValueImplementation(const arma::Col<double>& parameter) const {
    return arma::dot(scaling_, arma::square(getOscillationTransformation(rotationR_ * (parameter - translation_))));
  }
}
