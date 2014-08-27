#include <optimisationProblem/benchmark/rastriginFunction.hpp>

#include <cmath>

namespace hop {
  RastriginFunction::RastriginFunction(const unsigned int &numberOfDimensions) : BenchmarkProblem(numberOfDimensions), _delta(getScaling(std::sqrt(10.0))) {

  }

  double RastriginFunction::getObjectiveValueImplementation(const arma::Col<double> &parameter) const {
    arma::Col<double> z = _delta % getAsymmetricTransformation(0.2, getOscillationTransformation(parameter - _translation));

    return 10.0 * (static_cast<double>(_numberOfDimensions) - arma::accu(arma::cos(2.0 * arma::datum::pi * z))) + std::pow(arma::norm(z), 2);
  }
}
