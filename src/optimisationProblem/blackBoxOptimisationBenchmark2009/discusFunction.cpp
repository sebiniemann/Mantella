#include <mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2009/discusFunction.hpp>

// Cereal
#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

namespace mant {
  namespace bbob2009 {
    double DiscusFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const  {
      const arma::Col<double>& z = arma::square(getOscillationTransformation(rotationR_ * (parameter - translation_)));
      return 1000000.0 * z.at(0) + arma::accu(z.tail(z.n_elem - 1));
    }

    std::string DiscusFunction::to_string() const  {
      return "DiscusFunction";
    }
  }
}

CEREAL_REGISTER_TYPE(mant::bbob2009::DiscusFunction)
