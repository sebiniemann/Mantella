#include <optimisationProblem/benchmark/sphereFunction.hpp>

#include <cmath>

namespace hop {
  SphereFunction::SphereFunction(const unsigned int &numberOfDimensions) : BenchmarkProblem(numberOfDimensions) {

  }

  double SphereFunction::getObjectiveValueImplementation(const arma::Col<double> &parameter) const {
    return std::pow(arma::norm(parameter - _translation), 2);
  }
}
