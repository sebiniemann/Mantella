#include <hop_bits/optimisationProblem/benchmark/bbob2013/rastriginFunction.hpp>

#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

namespace hop {
  double RastriginFunction::getObjectiveValueImplementation(const arma::Col<double>& parameter) const {
    arma::Col<double> z = delta_ % getAsymmetricTransformation(0.2, getOscillationTransformation(parameter - translation_));

    return 10.0 * (static_cast<double>(numberOfDimensions_) - arma::accu(arma::cos(2.0 * arma::datum::pi * z))) + std::pow(arma::norm(z), 2);
  }

  std::string RastriginFunction::to_string() const {
    return "RastriginFunction";
  }
}

CEREAL_REGISTER_TYPE(hop::RastriginFunction)
