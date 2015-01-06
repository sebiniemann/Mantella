#include <hop_bits/optimisationProblem/blackBoxOptimisationBenchmark2010/rastriginFunctionRotated.hpp>

// Cereal
#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

namespace hop {
  namespace bbob2010 {
    double RastriginFunctionRotated::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      const arma::Col<double>& z = rotationR_ * (delta_ % (rotationQ_ * getAsymmetricTransformation(0.2, getOscillationTransformation(rotationR_ * (parameter - translation_)))));

      return 10.0 * (static_cast<double>(numberOfDimensions_) - arma::accu(arma::cos(2.0 * arma::datum::pi * z))) + std::pow(arma::norm(z), 2.0);
    }

    std::string RastriginFunctionRotated::to_string() const noexcept {
      return "RastriginFunctionRotated";
    }
  }
}

CEREAL_REGISTER_TYPE(hop::bbob2010::RastriginFunctionRotated)
