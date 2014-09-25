#include <hop_bits/optimisationProblem/benchmark/bbob2013/stepEllipsoidalFunction.hpp>

#include <algorithm>

#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

namespace hop {
  double StepEllipsoidalFunction::getObjectiveValueImplementation(const arma::Col<double>& parameter) const {
    arma::Col<double> zHat = delta_ % (rotationR_ * (parameter - translation_));

    arma::Col<double> zTilde(zHat);
    for(std::size_t n = 0; n < zTilde.n_elem; ++n) {
      double value = zHat.at(n);

      if(std::abs(value) > 0.5) {
        zTilde.at(n) = std::round(value);
      } else {
        zTilde.at(n) = std::round(value * 10.0) / 10.0;
      }
    }

    return 0.1 * std::max(std::abs(zHat.at(0)) / 10000.0, arma::dot(scaling_, arma::square(rotationQ_ * zTilde))) + getPenality(parameter);
  }

  std::string StepEllipsoidalFunction::to_string() const {
    return "StepEllipsoidalFunction";
  }
}

CEREAL_REGISTER_TYPE(hop::StepEllipsoidalFunction)
