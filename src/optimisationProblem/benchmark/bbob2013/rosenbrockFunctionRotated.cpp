#include <hop_bits/optimisationProblem/benchmark/bbob2013/rosenbrockFunctionRotated.hpp>

#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

namespace hop {
  double RosenbrockFunctionRotated::getObjectiveValueImplementation(const arma::Col<double>& parameter) const {
    arma::Col<double> z = max_ * rotationR_ * parameter + 0.5;

    return 100.0 * arma::accu(arma::square(arma::square(z.subvec(0, z.n_elem - 2)) - z.subvec(1, z.n_elem - 1))) + arma::accu(arma::square(z.subvec(0, z.n_elem - 2) - 1.0));
  }

  std::string RosenbrockFunctionRotated::to_string() const {
    return "RosenbrockFunctionRotated";
  }
}

CEREAL_REGISTER_TYPE(hop::RosenbrockFunctionRotated)
