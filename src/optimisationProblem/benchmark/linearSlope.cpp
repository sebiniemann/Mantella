#include <hop_bits/optimisationProblem/benchmark/linearSlope.hpp>

namespace hop {
  LinearSlope::LinearSlope(const unsigned int &numberOfDimensions) : BenchmarkProblem(numberOfDimensions) {

  }

  void LinearSlope::setOne(const arma::Col<double>& one) {
    _one = one;
    _xOpt = 5.0 * _one;
    _scaling = arma::sign(_one) % getScaling(10);
    _partiallyObjectiveValue = 5.0 * arma::accu(arma::abs(_scaling));
  }

  double LinearSlope::getObjectiveValueImplementation(const arma::Col<double> &parameter) const {
    arma::Col<double> z(parameter);

    arma::Col<arma::uword> outOfBound = arma::find(_xOpt % z > 25.0);
    z.elem(outOfBound) %= _xOpt.elem(outOfBound);

    return _partiallyObjectiveValue - arma::dot(_scaling, z);
  }
}
