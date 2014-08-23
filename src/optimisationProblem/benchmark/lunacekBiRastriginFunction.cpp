#include <optimisationProblem/benchmark/lunacekBiRastriginFunction.hpp>

#include <algorithm>
using std::min;

#include <cmath>
using std::sqrt;

#include <armadillo>
using arma::diagmat;
using arma::sign;
using arma::square;
using arma::accu;
using arma::cos;
using arma::datum;

namespace hop {
  LunacekBiRastriginFunction::LunacekBiRastriginFunction(const unsigned int &numberOfDimensions) : BenchmarkProblem(numberOfDimensions), _delta(getScaling(sqrt(100.0))), _s(1.0 - 0.5 / (sqrt(static_cast<double>(numberOfDimensions) + 20.0) - 4.1)) {
    _mu1 = -sqrt((6.25 - 1) / _s);
  }

  double LunacekBiRastriginFunction::getObjectiveValueImplementation(const Col<double> &parameter) const {
    Col<double> xHat = 2 * sign(_one) % parameter;
    Col<double> z = _rotationQ * (_delta % (_rotationR * (xHat - 2.5)));

    return min(accu(square(xHat - 2.5)), static_cast<double>(_numberOfDimensions) + _s * accu(square(xHat - _mu1))) + 10.0 * (static_cast<double>(_numberOfDimensions) - accu(cos(2.0 * datum::pi * z))) + 10000.0 * getPenality(parameter);
  }
}
