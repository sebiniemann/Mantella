#include <optimisationProblem/benchmark/rastriginFunctionRotated.hpp>

#include <cmath>
using std::sqrt;
using std::pow;

#include <armadillo>
using arma::norm;
using arma::datum;
using arma::accu;
using arma::cos;
using arma::diagmat;

namespace hop {
  RastriginFunctionRotated::RastriginFunctionRotated(const unsigned int &numberOfDimensions) : BenchmarkProblem(numberOfDimensions), _delta(getScaling(sqrt(10.0))) {

  }

  double RastriginFunctionRotated::getObjectiveValueImplementation(const Col<double> &parameter) const {
    Col<double> z = _rotationR * (_delta % (_rotationQ * getAsymmetricTransformation(0.2, getOscillationTransformation(_rotationR * (parameter - _translation)))));

    return 10.0 * (static_cast<double>(_numberOfDimensions) - accu(cos(2.0 * datum::pi * z))) + pow(norm(z), 2);
  }
}
