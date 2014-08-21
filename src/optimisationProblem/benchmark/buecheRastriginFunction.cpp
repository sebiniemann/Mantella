#include <optimisationProblem/benchmark/buecheRastriginFunction.hpp>

#include <cstdlib>
using std::abs;

#include <cmath>
using std::sqrt;
using std::pow;

#include <armadillo>
using arma::norm;
using arma::datum;
using arma::cos;

namespace hop {
  BuecheRastriginFunction::BuecheRastriginFunction(const unsigned int &numberOfDimensions) : BenchmarkProblem(numberOfDimensions), _scaling(getScaling(sqrt(10.0))) {
    for (size_t n = 0; n < _randomTranslation.n_elem; n += 2) {
      _randomTranslation.at(n) = abs(_randomTranslation.at(n));
    }
  }

  double BuecheRastriginFunction::getObjectiveValueImplementation(const Col<double> &parameter) const {
    Col<double> z = _scaling % getOscillationTransformation(getRandomParameterTranslation(parameter));
    for (size_t n = 0; n < z.n_elem; n += 2) {
      if (z.at(n) > 0.0) {
        z.at(n) *= 10.0;
      }
    }

    return 10.0 * (static_cast<double>(_numberOfDimensions) - accu(cos(2.0 * datum::pi * z))) + pow(norm(z), 2) + 100 * getPenality(parameter);
  }
}
