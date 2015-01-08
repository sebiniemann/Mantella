#include <hop_bits/optimisationProblem/blackBoxOptimisationBenchmark2010/lunacekBiRastriginFunction.hpp>

// C++ Standard Library
#include <algorithm>

// Cereal
#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

namespace hop {
  namespace bbob2010 {
    double LunacekBiRastriginFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      const arma::Col<double>& xHat = 2.0 * arma::sign(one_) % parameter;
      const arma::Col<double>& z = rotationQ_ * (delta_ % (rotationR_ * (xHat - 2.5)));

      return std::min(arma::accu(arma::square(xHat - 2.5)), static_cast<double>(numberOfDimensions_) + s_ * arma::accu(arma::square(xHat - mu1_))) + 10.0 * (static_cast<double>(numberOfDimensions_) - arma::accu(arma::cos(2.0 * arma::datum::pi * z))) + 10000.0 * getPenality(parameter);
    }

    std::string LunacekBiRastriginFunction::to_string() const noexcept {
      return "LunacekBiRastriginFunction";
    }
  }
}

CEREAL_REGISTER_TYPE(hop::bbob2010::LunacekBiRastriginFunction)
