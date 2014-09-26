#include <hop_bits/optimisationProblem/benchmark/bbob2013/rastriginFunctionRotated.hpp>

#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

namespace hop {
  double RastriginFunctionRotated::getObjectiveValueImplementation(const arma::Col<double>& parameter) const {
    arma::Col<double> z = rotationR_ * (delta_ % (rotationQ_ * getAsymmetricTransformation(0.2, getOscillationTransformation(rotationR_ * (parameter - translation_)))));

    return 10.0 * (static_cast<double>(numberOfDimensions_) - arma::accu(arma::cos(2.0 * arma::datum::pi * z))) + std::pow(arma::norm(z), 2);
  }

  std::string RastriginFunctionRotated::to_string() const {
    return "RastriginFunctionRotated";
  }
}

CEREAL_REGISTER_TYPE(hop::RastriginFunctionRotated)
