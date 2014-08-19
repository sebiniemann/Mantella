#include <optimisationProblem/benchmark/attractiveSectorFunction.hpp>

#include <cmath>
using std::sqrt;
using std::pow;

#include <armadillo>
using arma::diagmat;
using arma::find;

namespace hop {
  AttractiveSectorFunction::AttractiveSectorFunction(const unsigned int &numberOfDimensions) : BenchmarkProblem(numberOfDimensions), _delta(getScaling(sqrt(10.0))), _rotationQ(getRandomRotation()), _rotationR(getRandomRotation()) {

  }

  double AttractiveSectorFunction::getObjectiveValueImplementation(const Col<double> &parameter) const {
    Col<double> z = _rotationQ * diagmat(_delta) * _rotationR * getRandomParameterTranslation(parameter);
    z.elem(find(z % _randomTranslation > 0)) *= 100.0;

    return pow(getOscillationTransformation(pow(norm(z), 2)), 0.9);
  }
}
