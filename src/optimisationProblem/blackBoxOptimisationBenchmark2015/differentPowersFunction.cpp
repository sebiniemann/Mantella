#include <mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2015/differentPowersFunction.hpp>

// Cereal
#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

namespace mant {
  namespace bbob2015 {
    double DifferentPowersFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      const arma::Col<double>& z = arma::abs(rotationR_ * (parameter - translation_));
      return arma::norm(z % getScaling(arma::square(z)));
    }

    std::string DifferentPowersFunction::to_string() const noexcept {
      return "DifferentPowersFunction";
    }
  }
}

CEREAL_REGISTER_TYPE(mant::bbob2015::DifferentPowersFunction)
