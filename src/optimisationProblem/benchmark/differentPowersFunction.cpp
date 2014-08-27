#include <optimisationProblem/benchmark/differentPowersFunction.hpp>

namespace hop {
  DifferentPowersFunction::DifferentPowersFunction(const unsigned int &numberOfDimensions) : BenchmarkProblem(numberOfDimensions) {

  }

  double DifferentPowersFunction::getObjectiveValueImplementation(const arma::Col<double> &parameter) const {
    arma::Col<double> z = arma::abs(_rotationR * (parameter - _translation));
    return arma::norm(z % getScaling(arma::square(z)));
  }
}
