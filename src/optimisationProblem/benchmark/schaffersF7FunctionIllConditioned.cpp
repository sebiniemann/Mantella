#include <optimisationProblem/benchmark/schaffersF7FunctionIllConditioned.hpp>

#include <cmath>
using std::sqrt;
using std::pow;

#include <armadillo>
using arma::pow;
using arma::accu;
using arma::sin;
using arma::mean;

namespace hop {
  SchaffersF7FunctionIllConditioned::SchaffersF7FunctionIllConditioned(const unsigned int &numberOfDimensions) : BenchmarkProblem(numberOfDimensions), _delta(getScaling(sqrt(1000.0))), _rotationR(getRandomRotation()), _rotationQ(getRandomRotation()) {

  }

  double SchaffersF7FunctionIllConditioned::getObjectiveValueImplementation(const Col<double> &parameter) const {
    Col<double> z = square(diagmat(_delta) * _rotationQ * getAsymmetricTransformation(0.5, _rotationR * getRandomParameterTranslation(parameter)));
    Col<double> s = pow(z.subvec(0, z.n_elem - 2) + z.subvec(1, z.n_elem - 1), 0.25);

    return pow(mean(s + s * square(sin(50 * pow(s, 0.8)))), 2) + 10.0 * getPenality(parameter);
  }
}
