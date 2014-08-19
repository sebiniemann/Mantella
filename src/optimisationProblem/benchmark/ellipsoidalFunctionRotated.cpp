#include <optimisationProblem/benchmark/ellipsoidalFunctionRotated.hpp>

#include <armadillo>
using arma::dot;
using arma::square;

namespace hop {
  EllipsoidalFunctionRotated::EllipsoidalFunctionRotated(const unsigned int &numberOfDimensions) : BenchmarkProblem(numberOfDimensions), _scaling(getScaling(1000000.0)), _rotationR(getRandomRotation()) {

  }

  double EllipsoidalFunctionRotated::getObjectiveValueImplementation(const Col<double> &parameter) const {
    return dot(_scaling, square(getOscillationTransformation(_rotationR * getRandomParameterTranslation(parameter))));
  }
}
