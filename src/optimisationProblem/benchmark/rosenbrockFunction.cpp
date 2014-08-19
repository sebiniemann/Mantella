#include <optimisationProblem/benchmark/rosenbrockFunction.hpp>

#include <algorithm>
using std::max;

#include <cmath>
using std::pow;

namespace hop {
  RosenbrockFunction::RosenbrockFunction(const unsigned int &numberOfDimensions) : BenchmarkProblem(numberOfDimensions), _max(max(1.0, sqrt(static_cast<double>(numberOfDimensions) / 8.0))) {

  }

  double RosenbrockFunction::getObjectiveValueImplementation(const Col<double> &parameter) const {
    Col<double> z = _max * getRandomParameterTranslation(parameter) + 1.0;

    double result = 0;
    for(size_t n = 0; n < z.n_elem - 1; n++) {
      double value = z.at(n);

      result += 100.0 * pow(pow(value, 2) - z.at(n + 1), 2) + pow(value - 1.0, 2);
    }

    return result;
  }
}
