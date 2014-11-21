#include <hop_bits/optimisationProblem/benchmark/blackBoxOptimisationBenchmark2013/differentPowersFunction.hpp>

// Cereal
#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

namespace hop {
  namespace bbob2013 {
    double DifferentPowersFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const {
      arma::Col<double> z = arma::abs(rotationR_ * (parameter - translation_));
      return arma::norm(z % getScaling(arma::square(z)));
    }

    std::string DifferentPowersFunction::to_string() const noexcept {
      return "DifferentPowersFunction";
    }
  }
}

CEREAL_REGISTER_TYPE(hop::bbob2013::DifferentPowersFunction)
