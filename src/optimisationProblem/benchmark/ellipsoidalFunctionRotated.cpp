#include <hop_bits/optimisationProblem/benchmark/ellipsoidalFunctionRotated.hpp>

namespace hop {
  EllipsoidalFunctionRotated::EllipsoidalFunctionRotated(const unsigned int &numberOfDimensions) : BenchmarkProblem(numberOfDimensions), _scaling(getScaling(1000000.0)) {

  }

  double EllipsoidalFunctionRotated::getObjectiveValueImplementation(const arma::Col<double> &parameter) const {
    return arma::dot(_scaling, arma::square(getOscillationTransformation(_rotationR * (parameter - _translation))));
  }
}
