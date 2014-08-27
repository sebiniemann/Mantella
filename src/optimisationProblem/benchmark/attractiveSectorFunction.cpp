#include <optimisationProblem/benchmark/attractiveSectorFunction.hpp>

#include <cmath>

namespace hop {
  AttractiveSectorFunction::AttractiveSectorFunction(const unsigned int &numberOfDimensions) : BenchmarkProblem(numberOfDimensions), _delta(getScaling(std::sqrt(10.0))) {

  }

  double AttractiveSectorFunction::getObjectiveValueImplementation(const arma::Col<double> &parameter) const {
    arma::Col<double> z = _rotationQ * (_delta % (_rotationR * (parameter - _translation)));
    z.elem(arma::find(z % _translation > 0)) *= 100.0;

    return std::pow(getOscillationTransformation(std::pow(norm(z), 2)), 0.9);
  }
}
