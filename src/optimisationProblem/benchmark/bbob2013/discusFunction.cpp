#include <hop_bits/optimisationProblem/benchmark/bbob2013/discusFunction.hpp>

#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

namespace hop {
  double DiscusFunction::getObjectiveValueImplementation(const arma::Col<double>& parameter) const {
    arma::Col<double> z = arma::square(getOscillationTransformation(rotationR_ * (parameter - translation_)));
    return 1000000 * z.at(0) + arma::accu(z.subvec(1, z.n_elem - 1));
  }

  std::string DiscusFunction::to_string() const {
    return "DiscusFunction";
  }
}

CEREAL_REGISTER_TYPE(hop::DiscusFunction)
