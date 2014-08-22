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
  LinearSlope::LinearSlope(const unsigned int &numberOfDimensions) : BenchmarkProblem(numberOfDimensions), _scaling(getScaling(10)), _xOpt(numberOfDimensions) {
    _xOpt = 5.0 * _one;
    _scaling %= sign(_one);
    _partiallyObjectiveValue = 5.0 * accu(abs(_scaling));
  }

  double LinearSlope::getObjectiveValueImplementation(const Col<double> &parameter) const {
    Col<double> z(parameter);

    Col<uword> outOfBound = find(_xOpt % z < 25.0);
    z.elem(outOfBound) %= _xOpt.elem(outOfBound);

    return _partiallyObjectiveValue - dot(_scaling, z);
  }
}
