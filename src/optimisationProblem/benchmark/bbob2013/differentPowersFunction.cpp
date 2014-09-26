#include <hop_bits/optimisationProblem/benchmark/bbob2013/differentPowersFunction.hpp>

#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

namespace hop {
  double DifferentPowersFunction::getObjectiveValueImplementation(const arma::Col<double>& parameter) const {
    arma::Col<double> z = arma::abs(rotationR_ * (parameter - translation_));
    return arma::norm(z % getScaling(arma::square(z)));
  }

  std::string DifferentPowersFunction::to_string() const {
    return "DifferentPowersFunction";
  }
}

CEREAL_REGISTER_TYPE(hop::DifferentPowersFunction)
