#include <hop_bits/optimisationProblem/benchmark/attractiveSectorFunction.hpp>

namespace hop {
  double AttractiveSectorFunction::getObjectiveValueImplementation(const arma::Col<double>& parameter) const {
    arma::Col<double> z = rotationQ_ * (delta_ % (rotationR_ * (parameter - translation_)));
    z.elem(arma::find(z % translation_ > 0)) *= 100.0;

    return std::pow(getOscillationTransformation(std::pow(norm(z), 2)), 0.9);
  }
}
