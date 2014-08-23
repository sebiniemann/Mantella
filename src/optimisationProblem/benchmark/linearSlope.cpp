#include <optimisationProblem/benchmark/linearSlope.hpp>

#include <cmath>
using std::sqrt;
using std::pow;

#include <armadillo>
using arma::sign;
using arma::find;
using arma::accu;
using arma::abs;
using arma::dot;

namespace hop {
  LinearSlope::LinearSlope(const unsigned int &numberOfDimensions) : BenchmarkProblem(numberOfDimensions) {

  }

  void LinearSlope::setOne(const Col<double>& one) {
    _one = one;
    _xOpt = 5.0 * _one;
    _scaling = sign(_one) % getScaling(10);
    _partiallyObjectiveValue = 5.0 * accu(abs(_scaling));
  }

  double LinearSlope::getObjectiveValueImplementation(const Col<double> &parameter) const {
    Col<double> z(parameter);

    Col<uword> outOfBound = find(_xOpt % z > 25.0);
    z.elem(outOfBound) %= _xOpt.elem(outOfBound);

    return _partiallyObjectiveValue - dot(_scaling, z);
  }
}
