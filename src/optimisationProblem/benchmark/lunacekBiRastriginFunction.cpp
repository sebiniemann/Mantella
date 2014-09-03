#include <hop_bits/optimisationProblem/benchmark/lunacekBiRastriginFunction.hpp>

#include <algorithm>
#include <cmath>

namespace hop {
  LunacekBiRastriginFunction::LunacekBiRastriginFunction(const unsigned int &numberOfDimensions) : BenchmarkProblem(numberOfDimensions), _delta(getScaling(std::sqrt(100.0))), _s(1.0 - 0.5 / (std::sqrt(static_cast<double>(numberOfDimensions) + 20.0) - 4.1)) {
    _mu1 = -std::sqrt((6.25 - 1) / _s);
  }

  double LunacekBiRastriginFunction::getObjectiveValueImplementation(const arma::Col<double> &parameter) const {
    arma::Col<double> xHat = 2 * arma::sign(_one) % parameter;
    arma::Col<double> z = _rotationQ * (_delta % (_rotationR * (xHat - 2.5)));

    return std::min(arma::accu(arma::square(xHat - 2.5)), static_cast<double>(_numberOfDimensions) + _s * arma::accu(arma::square(xHat - _mu1))) + 10.0 * (static_cast<double>(_numberOfDimensions) - arma::accu(arma::cos(2.0 * arma::datum::pi * z))) + 10000.0 * getPenality(parameter);
  }
}
