#include <optimisationProblem/benchmark/katsuuraFunction.hpp>

#include <cmath>
using std::sqrt;
using std::pow;
using std::round;

#include <cstdlib>
using std::abs;

#include <armadillo>
using arma::diagmat;

namespace hop {
  KatsuuraFunction::KatsuuraFunction(const unsigned int &numberOfDimensions) : BenchmarkProblem(numberOfDimensions), _delta(getScaling(sqrt(100.0))) {

  }

  double KatsuuraFunction::getObjectiveValueImplementation(const Col<double> &parameter) const {
    Col<double> z = _rotationQ * (_delta % (_rotationR * (parameter - _translation)));

    double product = 1.0;
    for (size_t n = 0; n < z.n_elem; n++) {
        double value = z.at(n);

        double sum = 0.0;
        for (unsigned int k = 1; k < 33; k++) {
            double power = pow(2.0, k);
            sum += abs(power * value - round(power * value)) / power;
        }

        product *= pow(1.0 + static_cast<double>(n + 1) * sum, (10.0 / pow(_numberOfDimensions, 1.2)));
    }

    return 10.0 / pow(_numberOfDimensions, 2) * (product - 1.0) + getPenality(parameter);
  }
}
