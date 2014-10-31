#include <hop_bits/optimisationProblem/benchmark/blackBoxOptimisationBenchmark2013/ellipsoidalFunction.hpp>

// Cereal
#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

namespace hop {
  namespace bbob2013 {
    double EllipsoidalFunction::getObjectiveValueImplementation(const arma::Col<double>& parameter) const {
      return arma::dot(scaling_, arma::square(getOscillationTransformation(parameter - translation_)));
    }

    std::string EllipsoidalFunction::to_string() const {
      return "EllipsoidalFunction";
    }
  }
}

CEREAL_REGISTER_TYPE(hop::bbob2013::EllipsoidalFunction)
