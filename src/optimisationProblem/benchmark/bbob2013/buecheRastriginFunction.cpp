#include <hop_bits/optimisationProblem/benchmark/bbob2013/buecheRastriginFunction.hpp>

#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

namespace hop {
  void BuecheRastriginFunction::setTranslation(const arma::Col<double>& translation) {
    translation_ = translation;
    for (std::size_t n = 0; n < translation_.n_elem; n += 2) {
      translation_.at(n) = std::abs(translation_.at(n));
    }
  }

  double BuecheRastriginFunction::getObjectiveValueImplementation(const arma::Col<double>& parameter) const {
    arma::Col<double> z = scaling_ % getOscillationTransformation(parameter - translation_);
    for (std::size_t n = 0; n < z.n_elem; n += 2) {
      if (z.at(n) > 0.0) {
        z.at(n) *= 10.0;
      }
    }

    return 10.0 * (static_cast<double>(numberOfDimensions_) - arma::accu(arma::cos(2.0 * arma::datum::pi * z))) + std::pow(arma::norm(z), 2) + 100.0 * getPenality(parameter);
  }

  std::string BuecheRastriginFunction::to_string() const {
    return "BuecheRastriginFunction";
  }
}

CEREAL_REGISTER_TYPE(hop::BuecheRastriginFunction)
