#include <hop_bits/optimisationProblem/benchmark/buecheRastriginFunction.hpp>

#include <cmath>

namespace hop {
  BuecheRastriginFunction::BuecheRastriginFunction(const unsigned int &numberOfDimensions) : BenchmarkProblem(numberOfDimensions), _scaling(getScaling(sqrt(10.0))) {

  }

  void BuecheRastriginFunction::setTranslation(const arma::Col<double> &translation) {
    _translation = translation;
    for (std::size_t n = 0; n < _translation.n_elem; n += 2) {
      _translation.at(n) = std::abs(_translation.at(n));
    }
  }

  double BuecheRastriginFunction::getObjectiveValueImplementation(const arma::Col<double> &parameter) const {
    arma::Col<double> z = _scaling % getOscillationTransformation(parameter - _translation);
    for (std::size_t n = 0; n < z.n_elem; n += 2) {
      if (z.at(n) > 0.0) {
        z.at(n) *= 10.0;
      }
    }

    return 10.0 * (static_cast<double>(_numberOfDimensions) - arma::accu(arma::cos(2.0 * arma::datum::pi * z))) + std::pow(arma::norm(z), 2) + 100.0 * getPenality(parameter);
  }
}
