#include <mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2009/ellipsoidalFunction.hpp>

// Cereal
#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

namespace mant {
  namespace bbob2009 {
    double EllipsoidalFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const  {
      return arma::dot(scaling_, arma::square(getOscillationTransformation(parameter - translation_)));
    }

    std::string EllipsoidalFunction::to_string() const  {
      return "EllipsoidalFunction";
    }
  }
}

CEREAL_REGISTER_TYPE(mant::bbob2009::EllipsoidalFunction)
