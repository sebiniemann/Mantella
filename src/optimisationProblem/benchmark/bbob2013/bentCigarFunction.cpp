#include <hop_bits/optimisationProblem/benchmark/bbob2013/bentCigarFunction.hpp>

#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

namespace hop {
  double BentCigarFunction::getObjectiveValueImplementation(const arma::Col<double>& parameter) const {
    arma::Col<double> z = arma::square(rotationR_ * getAsymmetricTransformation(0.5, rotationR_ * (parameter - translation_)));
    return z.at(0) + 1000000.0 * arma::accu(z.subvec(1, z.n_elem - 1));
  }

  std::string BentCigarFunction::to_string() const {
    return "BentCigarFunction";
  }
}

CEREAL_REGISTER_TYPE(hop::BentCigarFunction)
