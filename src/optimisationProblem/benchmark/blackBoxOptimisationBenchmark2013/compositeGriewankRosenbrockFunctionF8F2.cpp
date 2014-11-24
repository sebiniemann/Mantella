#include <hop_bits/optimisationProblem/benchmark/blackBoxOptimisationBenchmark2013/compositeGriewankRosenbrockFunctionF8F2.hpp>

// Cereal
#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

namespace hop {
  namespace bbob2013 {
    double CompositeGriewankRosenbrockFunctionF8F2::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      const arma::Col<double>& z = max_ * rotationR_ * parameter + 0.5;
      const arma::Col<double>& s = 100.0 * arma::square(arma::square(z.subvec(0, z.n_elem - 2)) - z.subvec(1, z.n_elem - 1)) + arma::square(1.0 - z.subvec(0, z.n_elem - 2));

      return 10.0 * arma::mean(s / 4000.0 - arma::cos(s)) + 10.0;
    }

    std::string CompositeGriewankRosenbrockFunctionF8F2::to_string() const noexcept {
      return "CompositeGriewankRosenbrockFunctionF8F2";
    }
  }
}

CEREAL_REGISTER_TYPE(hop::bbob2013::CompositeGriewankRosenbrockFunctionF8F2)
