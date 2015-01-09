#include <mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2013/sphereFunction.hpp>

// C++ Standard Library
#include <cmath>

// Cereal
#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

namespace mant {
  namespace bbob2013 {
    double SphereFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      return std::pow(arma::norm(parameter - translation_), 2.0);
    }

    std::string SphereFunction::to_string() const noexcept {
      return "SphereFunction";
    }
  }
}

CEREAL_REGISTER_TYPE(mant::bbob2013::SphereFunction)
