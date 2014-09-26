#include <hop_bits/optimisationProblem/benchmark/bbob2013/ellipsoidalFunctionRotated.hpp>

#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

namespace hop {
  double EllipsoidalFunctionRotated::getObjectiveValueImplementation(const arma::Col<double>& parameter) const {
    return arma::dot(scaling_, arma::square(getOscillationTransformation(rotationR_ * (parameter - translation_))));
  }

  std::string EllipsoidalFunctionRotated::to_string() const {
    return "EllipsoidalFunctionRotated";
  }
}

CEREAL_REGISTER_TYPE(hop::EllipsoidalFunctionRotated)
