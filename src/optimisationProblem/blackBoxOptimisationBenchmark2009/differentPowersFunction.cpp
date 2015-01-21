#include <mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2009/differentPowersFunction.hpp>

// Cereal
#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

namespace mant {
  namespace bbob2009 {
    double DifferentPowersFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const  {
      const arma::Col<double>& z = arma::abs(rotationR_ * (parameter - translation_));
      return arma::norm(z % getScaling(arma::square(z)));
    }

    std::string DifferentPowersFunction::to_string() const  {
      return "DifferentPowersFunction";
    }
  }
}

CEREAL_REGISTER_TYPE(mant::bbob2009::DifferentPowersFunction)
