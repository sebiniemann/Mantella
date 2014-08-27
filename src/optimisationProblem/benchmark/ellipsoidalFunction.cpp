#include <optimisationProblem/benchmark/ellipsoidalFunction.hpp>

namespace hop {
  EllipsoidalFunction::EllipsoidalFunction(const unsigned int &numberOfDimensions) : BenchmarkProblem(numberOfDimensions), _scaling(getScaling(1000000.0)) {

  }

  double EllipsoidalFunction::getObjectiveValueImplementation(const arma::Col<double> &parameter) const {
    return arma::dot(_scaling, arma::square(getOscillationTransformation(parameter - _translation)));
  }
}
