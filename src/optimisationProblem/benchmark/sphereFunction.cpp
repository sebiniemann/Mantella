#include <hop_bits/optimisationProblem/benchmark/sphereFunction.hpp>

#include <cmath>

#include <hop_bits/helper/cereal.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

namespace hop {
  double SphereFunction::getObjectiveValueImplementation(const arma::Col<double>& parameter) const {
    return std::pow(arma::norm(parameter - translation_), 2);
  }
}

CEREAL_REGISTER_TYPE(hop::SphereFunction)
