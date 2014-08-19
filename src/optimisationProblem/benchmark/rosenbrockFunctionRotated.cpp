#include <optimisationProblem/benchmark/rosenbrockFunctionRotated.hpp>

#include <algorithm>
using std::max;

#include <cmath>
using std::pow;

namespace hop {
  RosenbrockFunctionRotated::RosenbrockFunctionRotated(const unsigned int &numberOfDimensions) : BenchmarkProblem(numberOfDimensions), _max(max(1.0, sqrt(static_cast<double>(numberOfDimensions) / 8.0))), _rotationR(getRandomRotation()) {

  }

  double RosenbrockFunctionRotated::getObjectiveValueImplementation(const Col<double> &parameter) const {
    Col<double> z = _max * _rotationR * parameter + 0.5;

    double result = 0;
    for(size_t n = 0; n < z.n_elem - 1; n++) {
      double value = z.at(n);

      result += 100.0 * pow(pow(value, 2) - z.at(n + 1), 2) + pow(value - 1.0, 2);
    }

    return result;
  }
}
