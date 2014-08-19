#include <optimisationProblem/benchmark/differentPowersFunction.hpp>

#include <cmath>
using std::sqrt;
using std::pow;

#include <armadillo>
using arma::abs;
using arma::norm;
using arma::square;

namespace hop {
  DifferentPowersFunction::DifferentPowersFunction(const unsigned int &numberOfDimensions) : BenchmarkProblem(numberOfDimensions), _rotationR(getRandomRotation()) {

  }

  double DifferentPowersFunction::getObjectiveValueImplementation(const Col<double> &parameter) const {
    Col<double> z = abs(_rotationR * getRandomParameterTranslation(parameter));
    return norm(z % getScaling(square(z)));
  }
}
