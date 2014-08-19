#include <optimisationProblem/benchmark/sharpRidgeFunction.hpp>

#include <cmath>
using std::sqrt;
using std::pow;

#include <armadillo>
using arma::diagmat;
using arma::norm;

namespace hop {
  SharpRidgeFunction::SharpRidgeFunction(const unsigned int &numberOfDimensions) : BenchmarkProblem(numberOfDimensions), _delta(getScaling(sqrt(10.0))), _rotationR(getRandomRotation()), _rotationQ(getRandomRotation()) {

  }

  double SharpRidgeFunction::getObjectiveValueImplementation(const Col<double> &parameter) const {
    Col<double> z = _rotationQ * diagmat(_delta) * _rotationR * getRandomParameterTranslation(parameter);
    return pow(z.at(0), 2) + 100 * norm(z.subvec(1, z.n_elem - 1));
  }
}
