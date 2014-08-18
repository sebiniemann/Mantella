#include <optimisationProblem/benchmark/rastriginFunction.hpp>

#include <cmath>
using std::sqrt;
using std::pow;

#include <armadillo>
using arma::norm;
using arma::datum;
using arma::cos;

namespace hop {
  RastriginFunction::RastriginFunction(const unsigned int &numberOfDimensions) : BenchmarkProblem(numberOfDimensions), _delta(getScaling(sqrt(10.0))) {

  }

  double RastriginFunction::getObjectiveValueImplementation(const Col<double> &parameter) {
    Col<double> z = _delta * getAsymmetricTransformation(0.2, getOscillationTransformation(getRandomParameterTranslation(parameter)));

    return 10.0 * (static_cast<double>(_numberOfDimensions) - accu(cos(2 * datum::pi * z))) + pow(norm(z), 2);
  }
}
