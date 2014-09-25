#include <hop_bits/optimisationProblem/benchmark/bbob2013/attractiveSectorFunction.hpp>

#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

namespace hop {
  double AttractiveSectorFunction::getObjectiveValueImplementation(const arma::Col<double>& parameter) const {
    arma::Col<double> z = rotationQ_ * (delta_ % (rotationR_ * (parameter - translation_)));
    z.elem(arma::find(z % translation_ > 0)) *= 100.0;

    return std::pow(getOscillationTransformation(std::pow(norm(z), 2)), 0.9);
  }

  std::string AttractiveSectorFunction::to_string() const {
    return "AttractiveSectorFunction";
  }
}

CEREAL_REGISTER_TYPE(hop::AttractiveSectorFunction)
