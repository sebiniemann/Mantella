#include <optimisationProblem/benchmark/buecheRastriginFunction.hpp>

#include <cmath>
using std::sqrt;
using std::pow;

#include <armadillo>
using arma::norm;
using arma::datum;
using arma::cos;

namespace hop {
  BuecheRastriginFunction::BuecheRastriginFunction(const unsigned int &numberOfDimensions) : BenchmarkProblem(numberOfDimensions), _scaling(getScaling(sqrt(10))) {

  }

  double BuecheRastriginFunction::getObjectiveValueImplementation(const Col<double> &parameter) {
    Col<double> z = _scaling % getOscillationTransformation(getRandomParameterTranslation(parameter));
    for (size_t n = 0; n < z.n_elem; n += 2) {
      if (z.at(n) > 0) {
        z.at(n) *= 10;
      }
    }

    return 10.0 * (static_cast<double>(_numberOfDimensions) - accu(cos(2 * datum::pi * z))) + pow(norm(z), 2);
  }
}
