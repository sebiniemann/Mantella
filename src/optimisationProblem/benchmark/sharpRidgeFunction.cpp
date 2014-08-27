#include <optimisationProblem/benchmark/sharpRidgeFunction.hpp>

#include <cmath>

namespace hop {
  SharpRidgeFunction::SharpRidgeFunction(const unsigned int &numberOfDimensions) : BenchmarkProblem(numberOfDimensions), _delta(getScaling(std::sqrt(10.0))) {

  }

  double SharpRidgeFunction::getObjectiveValueImplementation(const arma::Col<double> &parameter) const {
    arma::Col<double> z = _rotationQ * (_delta % (_rotationR * (parameter - _translation)));
    return std::pow(z.at(0), 2) + 100 * arma::norm(z.subvec(1, z.n_elem - 1));
  }
}
