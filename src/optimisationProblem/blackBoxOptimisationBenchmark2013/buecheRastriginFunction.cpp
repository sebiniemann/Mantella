#include <hop_bits/optimisationProblem/blackBoxOptimisationBenchmark2013/buecheRastriginFunction.hpp>

// Cereal
#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

namespace hop {
  namespace bbob2013 {
    BuecheRastriginFunction::BuecheRastriginFunction(
        const unsigned int& numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark2013(numberOfDimensions) {
      setTranslation(translation_);
    }

    void BuecheRastriginFunction::setTranslation(
        const arma::Col<double>& translation) {
      if (translation.n_elem != numberOfDimensions_) {
        throw std::logic_error("The number of dimensions of the translation (" + std::to_string(translation.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
      }

      translation_ = translation;
      for (std::size_t n = 0; n < translation_.n_elem; n += 2) {
        translation_.at(n) = std::abs(translation_.at(n));
      }
    }

    double BuecheRastriginFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      arma::Col<double> z = scaling_ % getOscillationTransformation(parameter - translation_);
      for (std::size_t n = 0; n < z.n_elem; n += 2) {
        if (z.at(n) > 0.0) {
          z.at(n) *= 10.0;
        }
      }

      return 10.0 * (static_cast<double>(numberOfDimensions_) - arma::accu(arma::cos(2.0 * arma::datum::pi * z))) + std::pow(arma::norm(z), 2.0) + 100.0 * getPenality(parameter);
    }

    std::string BuecheRastriginFunction::to_string() const noexcept {
      return "BuecheRastriginFunction";
    }
  }
}

CEREAL_REGISTER_TYPE(hop::bbob2013::BuecheRastriginFunction)
