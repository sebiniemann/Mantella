#include <hop_bits/optimisationProblem/benchmark/blackBoxOptimisationBenchmark2013/sharpRidgeFunction.hpp>

// Cereal
#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

namespace hop {
  namespace bbob2013 {
    double SharpRidgeFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const {
      arma::Col<double> z = rotationQ_ * (delta_ % (rotationR_ * (parameter - translation_)));
      return std::pow(z.at(0), 2) + 100 * arma::norm(z.subvec(1, z.n_elem - 1));
    }

    std::string SharpRidgeFunction::to_string() const noexcept {
      return "SharpRidgeFunction";
    }
  }
}

CEREAL_REGISTER_TYPE(hop::bbob2013::SharpRidgeFunction)
