#include <mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2009/sphereFunction.hpp>

// C++ Standard Library
#include <cmath>

// Cereal
#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

namespace mant {
  namespace bbob2009 {
    double SphereFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const  {
      return std::pow(arma::norm(parameter - translation_), 2.0);
    }

    std::string SphereFunction::to_string() const  {
      return "SphereFunction";
    }
  }
}

CEREAL_REGISTER_TYPE(mant::bbob2009::SphereFunction)
