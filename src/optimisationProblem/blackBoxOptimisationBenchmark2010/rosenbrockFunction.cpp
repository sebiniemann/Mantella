#include <mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2010/rosenbrockFunction.hpp>

// Cereal
#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

namespace mant {
  namespace bbob2010 {
    RosenbrockFunction::RosenbrockFunction(
        const unsigned int& numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark2010(numberOfDimensions) {
      setTranslation(translation_);
    }

    void RosenbrockFunction::setTranslation(
        const arma::Col<double>& translation) {
      if (translation.n_elem != numberOfDimensions_) {
        throw std::logic_error("The number of dimensions of the translation (" + std::to_string(translation.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
      }

      translation_ = 0.75 * translation;
    }

    double RosenbrockFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      const arma::Col<double>& z = max_ * (parameter - translation_) + 1.0;

      return 100.0 * arma::accu(arma::square(arma::square(z.head(z.n_elem - 1)) - z.tail(z.n_elem - 1))) + arma::accu(arma::square(z.head(z.n_elem - 1) - 1.0));
    }

    std::string RosenbrockFunction::to_string() const noexcept {
      return "RosenbrockFunction";
    }
  }
}

CEREAL_REGISTER_TYPE(mant::bbob2010::RosenbrockFunction)
