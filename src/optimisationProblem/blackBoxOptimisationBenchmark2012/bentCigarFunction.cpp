#include <mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2012/bentCigarFunction.hpp>

// Cereal
#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

namespace mant {
  namespace bbob2012 {
    double BentCigarFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      const arma::Col<double>& z = arma::square(rotationR_ * getAsymmetricTransformation(0.5, rotationR_ * (parameter - translation_)));
      return z.at(0) + 1000000.0 * arma::accu(z.tail(z.n_elem - 1));
    }

    std::string BentCigarFunction::to_string() const noexcept {
      return "BentCigarFunction";
    }
  }
}

CEREAL_REGISTER_TYPE(mant::bbob2012::BentCigarFunction)
