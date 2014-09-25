#include <hop_bits/optimisationProblem/benchmark/bbob2013/sphereFunction.hpp>

#include <cmath>

#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

namespace hop {
  double SphereFunction::getObjectiveValueImplementation(const arma::Col<double>& parameter) const {
    return std::pow(arma::norm(parameter - translation_), 2);
  }

  std::string SphereFunction::to_string() const {
    return "SphereFunction";
  }
}

CEREAL_REGISTER_TYPE(hop::SphereFunction)
