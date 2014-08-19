#include <optimisationProblem/benchmark/ellipsoidalFunction.hpp>

#include <armadillo>
using arma::dot;
using arma::square;

namespace hop {
  EllipsoidalFunction::EllipsoidalFunction(const unsigned int &numberOfDimensions) : BenchmarkProblem(numberOfDimensions), _scaling(getScaling(1000000.0)) {

  }

  double EllipsoidalFunction::getObjectiveValueImplementation(const Col<double> &parameter) const {
    return dot(_scaling, square(getOscillationTransformation(getRandomParameterTranslation(parameter))));
  }
}
