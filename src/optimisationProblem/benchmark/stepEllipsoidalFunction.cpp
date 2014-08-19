#include <optimisationProblem/benchmark/stepEllipsoidalFunction.hpp>

#include <algorithm>
using std::max;

#include <cmath>
using std::abs;
using std::round;

#include <armadillo>
using arma::diagmat;
using arma::find;
using arma::dot;
using arma::square;

namespace hop {
  StepEllipsoidalFunction::StepEllipsoidalFunction(const unsigned int &numberOfDimensions) : BenchmarkProblem(numberOfDimensions), _scaling(getScaling(100.0)), _delta(getScaling(sqrt(10.0))), _rotationQ(getRandomRotation()), _rotationR(getRandomRotation()) {

  }

  double StepEllipsoidalFunction::getObjectiveValueImplementation(const Col<double> &parameter) const {
    Col<double> zHat = diagmat(_delta) * _rotationR * getRandomParameterTranslation(parameter);

    Col<double> zTilde(zHat.n_elem);
    for(size_t n = 0; n < zTilde.n_elem; n++) {
      double value = zHat.at(n);

      if(value > 0.5) {
        zTilde.at(n) = round(value);
      } else {
        zTilde.at(n) = round(value * 10.0) / 10.0;
      }
    }

    return 0.1 * max(abs(zHat.at(0)) / 10000.0, dot(_scaling, square(_rotationQ * zTilde))) + getPenality(parameter);
  }
}
