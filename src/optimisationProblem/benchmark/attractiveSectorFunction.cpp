#include <optimisationProblem/benchmark/attractiveSectorFunction.hpp>

#include <cmath>
using std::sqrt;
using std::pow;

#include <armadillo>
using arma::diagmat;
using arma::find;

namespace hop {
  AttractiveSectorFunction::AttractiveSectorFunction(const unsigned int &numberOfDimensions) : BenchmarkProblem(numberOfDimensions), _delta(getScaling(sqrt(10.0))) {

  }

  double AttractiveSectorFunction::getObjectiveValueImplementation(const Col<double> &parameter) const {
    Col<double> z = _rotationQ * (_delta % (_rotationR * (parameter - _translation)));
    z.elem(find(z % _translation > 0)) *= 100.0;

    return pow(getOscillationTransformation(pow(norm(z), 2)), 0.9);
  }
}
