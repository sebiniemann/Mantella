#include <optimisationProblem/benchmark/differentPowersFunction.hpp>

#include <cmath>
using std::sqrt;
using std::pow;

#include <armadillo>
using arma::abs;
using arma::norm;
using arma::square;

namespace hop {
  DifferentPowersFunction::DifferentPowersFunction(const unsigned int &numberOfDimensions) : BenchmarkProblem(numberOfDimensions) {

  }

  double DifferentPowersFunction::getObjectiveValueImplementation(const Col<double> &parameter) const {
    Col<double> z = abs(_rotationR * (parameter - _translation));
    return norm(z % getScaling(square(z)));
  }
}
