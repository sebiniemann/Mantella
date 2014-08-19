#include <optimisationProblem/benchmark/sphereFunction.hpp>

#include <cmath>
using std::sqrt;
using std::pow;

#include <armadillo>
using arma::norm;

namespace hop {
  SphereFunction::SphereFunction(const unsigned int &numberOfDimensions) : BenchmarkProblem(numberOfDimensions) {

  }

  double SphereFunction::getObjectiveValueImplementation(const Col<double> &parameter) const {
    return pow(norm(getRandomParameterTranslation(parameter)), 2);
  }
}
