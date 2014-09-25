#include <hop_bits/optimisationProblem/benchmark/bbob2013/lunacekBiRastriginFunction.hpp>

#include <algorithm>

#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

namespace hop {
  double LunacekBiRastriginFunction::getObjectiveValueImplementation(const arma::Col<double>& parameter) const {
    arma::Col<double> xHat = 2 * arma::sign(one_) % parameter;
    arma::Col<double> z = rotationQ_ * (delta_ % (rotationR_ * (xHat - 2.5)));

    return std::min(arma::accu(arma::square(xHat - 2.5)), static_cast<double>(numberOfDimensions_) + s_ * arma::accu(arma::square(xHat - mu1_))) + 10.0 * (static_cast<double>(numberOfDimensions_) - arma::accu(arma::cos(2.0 * arma::datum::pi * z))) + 10000.0 * getPenality(parameter);
  }

  std::string LunacekBiRastriginFunction::to_string() const {
    return "LunacekBiRastriginFunction";
  }
}

CEREAL_REGISTER_TYPE(hop::LunacekBiRastriginFunction)
