#include <hop_bits/optimisationProblem/blackBoxOptimisationBenchmark2015/sharpRidgeFunction.hpp>

// Cereal
#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

namespace hop {
  namespace bbob2015 {
    double SharpRidgeFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      const arma::Col<double>& z = rotationQ_ * (delta_ % (rotationR_ * (parameter - translation_)));
      return std::pow(z.at(0), 2.0) + 100.0 * arma::norm(z.tail(z.n_elem - 1));
    }

    std::string SharpRidgeFunction::to_string() const noexcept {
      return "SharpRidgeFunction";
    }
  }
}

CEREAL_REGISTER_TYPE(hop::bbob2015::SharpRidgeFunction)
