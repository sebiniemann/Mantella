#include <hop_bits/optimisationProblem/blackBoxOptimisationBenchmark2010/discusFunction.hpp>

// Cereal
#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

namespace hop {
  namespace bbob2010 {
    double DiscusFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      const arma::Col<double>& z = arma::square(getOscillationTransformation(rotationR_ * (parameter - translation_)));
      return 1000000.0 * z.at(0) + arma::accu(z.tail(z.n_elem - 1));
    }

    std::string DiscusFunction::to_string() const noexcept {
      return "DiscusFunction";
    }
  }
}

CEREAL_REGISTER_TYPE(hop::bbob2010::DiscusFunction)
