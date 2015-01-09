#include <mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2009/rosenbrockFunctionRotated.hpp>

// Cereal
#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

namespace mant {
  namespace bbob2009 {
    double RosenbrockFunctionRotated::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      const arma::Col<double>& z = max_ * rotationR_ * parameter + 0.5;

      return 100.0 * arma::accu(arma::square(arma::square(z.head(z.n_elem - 1)) - z.tail(z.n_elem - 1))) + arma::accu(arma::square(z.head(z.n_elem - 1) - 1.0));
    }

    std::string RosenbrockFunctionRotated::to_string() const noexcept {
      return "RosenbrockFunctionRotated";
    }
  }
}

CEREAL_REGISTER_TYPE(mant::bbob2009::RosenbrockFunctionRotated)
