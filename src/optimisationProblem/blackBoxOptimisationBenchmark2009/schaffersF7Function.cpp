#include <mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2009/schaffersF7Function.hpp>

// Cereal
#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

namespace mant {
  namespace bbob2009 {
    double SchaffersF7Function::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      const arma::Col<double>& z = arma::square(delta_ % (rotationQ_ * getAsymmetricTransformation(0.5, rotationR_ * (parameter - translation_))));
      const arma::Col<double>& s = arma::pow(z.head(z.n_elem - 1) + z.tail(z.n_elem - 1), 0.25);

      return std::pow(arma::mean(s % (1.0 + arma::square(arma::sin(50.0 * arma::pow(s, 0.4))))), 2.0) + 10.0 * getPenality(parameter);
    }

    std::string SchaffersF7Function::to_string() const noexcept {
      return "SchaffersF7Function";
    }
  }
}

CEREAL_REGISTER_TYPE(mant::bbob2009::SchaffersF7Function)
