#include <hop_bits/optimisationProblem/benchmark/stepEllipsoidalFunction.hpp>

#include <algorithm>
#include <cmath>

namespace hop {
  StepEllipsoidalFunction::StepEllipsoidalFunction(const unsigned int &numberOfDimensions) : BenchmarkProblem(numberOfDimensions), _scaling(getScaling(100.0)), _delta(getScaling(std::sqrt(10.0))) {

  }

  double StepEllipsoidalFunction::getObjectiveValueImplementation(const arma::Col<double> &parameter) const {
    arma::Col<double> zHat = _delta % (_rotationR * (parameter - _translation));

    arma::Col<double> zTilde(zHat);
    for(std::size_t n = 0; n < zTilde.n_elem; n++) {
      double value = zHat.at(n);

      if(std::abs(value) > 0.5) {
        zTilde.at(n) = std::round(value);
      } else {
        zTilde.at(n) = std::round(value * 10.0) / 10.0;
      }
    }

    return 0.1 * std::max(std::abs(zHat.at(0)) / 10000.0, arma::dot(_scaling, arma::square(_rotationQ * zTilde))) + getPenality(parameter);
  }
}
